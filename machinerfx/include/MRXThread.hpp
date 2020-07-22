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
 * File: MRXThread.hpp
 * Project: MachineRX
 * Author: Ali AlSaibie (ali.alsaibie@ku.edu.kw)
 * -----
 * Modified By: Ali AlSaibie (ali.alsaibie@ku.edu.kw>)
 */

#ifndef _MRFXThread_HPP_
#define _MRFXThread_HPP_
#include "MRXUtil.hpp"
#ifdef __cpluplus
extern "C" {
#endif
#include <assert.h>

#include "FreeRTOS.h"
#include "task.h"
#ifdef __cpluplus
}
#endif

namespace MachineRFX {
enum class ThreadStatus_n {
    Thread_Healthy = 0x00,
    Thread_Period_Missed = 0x01,
    Thread_Duration_Overflow = 0x02,
    Thread_Sleep = 0x04
};

class MRXThread {
   public:
    inline uint32_t start() {
        // TODO: Add check guards
        // TODO: cast priority type 

        if (xTaskCreate(&task, thread_name, thread_stack_depth, (void *)this, (int)thread_priority, &task_handle) ==
            pdPASS) {
            return pdPASS;
        } else {
            return pdFAIL;
        }
    }
    inline char *getName() {
        return pcTaskGetName(task_handle);
    }
    inline MRXPriority_n getPriority(){
        return thread_priority;
        //TODO: explicit get priority call from FreeRTOS
    }

   protected:
    MRXThread(const char *name, uint32_t stack_depth, MRXPriority_n priority, uint32_t Ts_ms)
        : thread_stack_depth(stack_depth),
          thread_priority(priority),
          t_sampling_ms(Ts_ms),
          t_sampling_ticks(pdMS_TO_TICKS(Ts_ms)),
          initializedDelay(false) {
        thread_name = name;
    };
    ~MRXThread() {
    }

    inline static void task(void *arg) {
        MRXThread *ptr = static_cast<MRXThread *>(arg);
        ptr->run();
    }

    virtual void run() { /* Redefine this function when inheriting */
        /* Do thread stuff */
        thread_lap(); /* Call to complete the thread period */
    }

    inline void thread_lap() {
        if (!initializedDelay) {
            initializedDelay = true;
            previousWakeTick = xTaskGetTickCount();
        }
        task_excution_ticks = xTaskGetTickCount() - thread_start_ticks;
        thread_status = ThreadStatus_n::Thread_Healthy;
        if (task_excution_ticks > t_sampling_ticks) {
            thread_status = ThreadStatus_n::Thread_Duration_Overflow;
        }
        vTaskDelayUntil(&previousWakeTick, t_sampling_ticks / portTICK_PERIOD_MS);
        thread_start_ticks = xTaskGetTickCount();
    }

    uint32_t t_sampling_ms;
   private:
    const char *thread_name;
    TaskHandle_t task_handle;
    uint32_t thread_stack_depth;
    MRXPriority_n thread_priority;
    TickType_t t_sampling_ticks;
    bool initializedDelay;
    TickType_t previousWakeTick{0};
    TickType_t thread_start_ticks{0};
    TickType_t task_excution_ticks{0};
    ThreadStatus_n thread_status;
};

}  // namespace MachineRFX

#endif  // _MRFXThread_HPP_