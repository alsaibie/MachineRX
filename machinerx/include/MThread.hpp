/*
 * MIT License
 * 
 * Copyright (c) 2020 Ali AlSaibie
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 * 
 * 
 * File: MThread.hpp
 * Project: MachineRX
 * Author: Ali AlSaibie (ali.alsaibie@ku.edu.kw)
 * -----
 * Modified By: Ali AlSaibie (ali.alsaibie@ku.edu.kw>)
 */
#ifndef _MTHREAD_HPP_
#define _MTHREAD_HPP_
#ifdef __cpluplus
extern "C" {
#endif


// #include <mqueue.h>
#include <assert.h>
#include <limits.h>

#ifdef __cpluplus
}
#endif

#include <stdint.h>

#ifdef configMAX_TASK_NAME_LEN
    #define MAX_THREAD_NAME_SIZE configMAX_TASK_NAME_LEN
#else
    #define MAX_THREAD_NAME_SIZE (20)
#endif

namespace MachineRX {

enum class ThreadStatus_t {
    Thread_Healthy = 0x00,
    Thread_Period_Missed = 0x01,
    Thread_Duration_Overflow = 0x02,
    Thread_Sleep = 0x04
};

class MThread {
   public:
    inline uint32_t start() {
        uint32_t ret;
        sched_param schparam;

        ret = pthread_attr_init(&thread_attr);
        assert(ret == 0);

        ret = pthread_attr_getschedparam(&thread_attr, &schparam);
        assert(ret == 0);
        schparam.sched_priority = thread_priority;

        /* Round-Robin Scheduling Policy */
        ret = pthread_attr_setschedpolicy(&thread_attr, SCHED_RR);
        assert(ret == 0);

        ret = pthread_attr_setschedparam(&thread_attr, &schparam);
        assert(ret == 0);

        ret = pthread_attr_setstacksize(&thread_attr, (thread_stack_depth <= PTHREAD_STACK_MIN) ? PTHREAD_STACK_MIN : thread_stack_depth);
        assert(ret == 0);

        ret = pthread_create(&thread_handle, &thread_attr, task, (void *)this);
        assert(ret == 0);

        ret = pthread_setname_np(thread_handle, thread_name);
        // assert(ret == 0);
    }

    inline const char *getThreadName() {
        uint32_t ret;

        // if (thread_name != NULL) {
        //     return thread_name;
        // }

        static char buf[MAX_THREAD_NAME_SIZE]{};

        ret = pthread_getname_np(thread_handle, buf, MAX_THREAD_NAME_SIZE);
        // assert(ret == 0);

        thread_name = buf;
        return thread_name;
    }

    inline const uint32_t getThreadPriority() {
        /* Explicit get priority call */
        struct sched_param param;
        int ret;

        ret = pthread_attr_getschedparam(&thread_attr, &param);
        assert(ret == 0);

        thread_priority = param.sched_priority;
        return thread_priority;
    }

   protected:
    MThread(const char *name, uint32_t stack_depth, uint32_t priority, uint32_t Ts_ms)
        : thread_stack_depth(stack_depth),
          thread_priority(priority),
          t_sampling_ms(Ts_ms),
          initializedDelay(false) {
        thread_name = name;
    };
    ~MThread() {
    }

    inline static void *task(void *arg) {
        MThread *ptr = static_cast<MThread *>(arg);
        ptr->run();
    }

    virtual void run() { /* Redefine this function when inheriting */
        /* Do thread stuff */
        thread_lap(); /* Call to complete the thread period */
    }

    inline void thread_lap() {
        uint32_t ret;
        if (!initializedDelay) {
            initializedDelay = true;
            ret = clock_gettime(CLOCK_MONOTONIC, &ts);
        }

        uint64_t next_tick = (ts.tv_sec * 1000L + ts.tv_nsec / 1000000) + t_sampling_ms;
        ts.tv_sec = next_tick / 1000L;
        ts.tv_nsec = (next_tick % 1000L) * 1000000L;

        clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &ts, NULL);

        // task_excution_ticks = xTaskGetTickCount() - thread_start_ticks;
        // thread_status = ThreadStatus_t::Thread_Healthy;
        // if (task_excution_ticks > t_sampling_ticks)
        // {
        //   thread_status = ThreadStatus_t::Thread_Duration_Overflow;
        // }
        // vTaskDelayUntil(&previousWakeTick, t_sampling_ticks / portTICK_PERIOD_MS);
        // thread_start_ticks = xTaskGetTickCount();
    }

    inline void msleep(uint32_t ms) {
        struct timespec ts_;
        ts_.tv_sec = ms / 1000L;
        ts_.tv_nsec = (ms % 1000L) * 1000000L;
        nanosleep(&ts_, NULL);
    }

   private:
    const char *thread_name;

    pthread_t thread_handle;
    pthread_attr_t thread_attr;
    uint32_t thread_stack_depth;
    uint32_t thread_priority;
    uint32_t t_sampling_ms;
    struct timespec ts {};
    bool initializedDelay;
    uint32_t thread_start_ns{0};
    uint32_t task_excution_ns{0};
    ThreadStatus_t thread_status;
};

}  // namespace MachineRX

#endif  //_MTHREAD_HPP_