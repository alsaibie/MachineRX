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
 * File: MTopic.hpp
 * Project: MRTOS
 * Author: Ali AlSaibie (ali.alsaibie@ku.edu.kw)
 * -----
 * Modified By: Ali AlSaibie (ali.alsaibie@ku.edu.kw>)
 */

#ifndef _MTOPIC_HPP_
#define _MTOPIC_HPP_

#include <cstdint>
#include <cstdio>
#include <cstring>

#ifdef __cpluplus
extern "C" {
#endif

#include <assert.h>
#include <pthread.h>
// #include <limits.h>

#ifdef __cpluplus
}
#endif

#include <functional>
#include <vector>

namespace MachineRX {

typedef uint32_t td_t;

typedef struct _msgCore {
    uint32_t tick_stamp_ms{0};
    uint32_t msg_count{0};
} MsgCore_T;

typedef struct {
    const td_t id{};
    const char *name;
    const uint32_t size;
} MTopicHandle_t;

class MTopicBase_ {
   public:
    inline const char *getName() {
        return th.name;
    }

    inline const td_t getId() {
        return th.id;
    }

    inline const uint32_t getSize() {
        return th.size;
    }

    /* Requires C++17 */
    inline static std::vector<MTopicHandle_t *> ptrMTH;
    inline static timespec ts_start{0, 0};
    inline static bool time_spec_offset{false};

   protected:
    MTopicBase_(MTopicHandle_t &th_, uint32_t msg_size) : th(th_) {
        if (NULL == th.id) {
            pthread_mutex_init(&msg_access_mutex, NULL);
            msgPtr = create_shared_memspace(msg_size);
            ptrMTH.push_back(&th);
        }
        if (time_spec_offset == false) {
            if (clock_gettime(CLOCK_MONOTONIC, &ts_start) != 0) {
                // fail TODO
            }
        }
    }
    ~MTopicBase_() {
        //TODO: pop out of vector ptrMTH
        destroy_shared_memspace();
    }

   private:
    inline void *create_shared_memspace(uint32_t size) {
        void *ptr = malloc(size);
        memset(ptr, 0, size);  //TODO: replace with custom per platform allocate
        return ptr;
    }

    inline void destroy_shared_memspace() {
        free(msgPtr);  //TODO: replace with custom per platform dealloc
    }

    inline uint32_t timespec_to_ms(struct timespec &ts_f, struct timespec &ts_i) {
        uint32_t t_ms = (ts_f.tv_sec - ts_i.tv_sec) * 1000L;
        int32_t delta_tms = (ts_f.tv_nsec / 1000000L - ts_i.tv_nsec / 1000000L);
        if (delta_tms >= 0) {
            t_ms += delta_tms;
        } else {
            t_ms += 1000 + delta_tms;
        }
        return t_ms;
    }

    MTopicHandle_t th;
    pthread_mutex_t msg_access_mutex;
    void *msgPtr;
};

template <typename MTopicMsgT>
class MTopicPublisher : public MTopicBase_ {
   public:
    MTopicPublisher(MTopicHandle_t &th) : MTopicBase_(th, sizeof(MTopicMsgT)) {
    }

    inline uint32_t publish(MTopicMsgT &msg) {
        MsgCore_T *msgCore = static_cast<MsgCore_T *>(&msg);
        msgCore->msg_count++;
        struct timespec ts;
        if (clock_gettime(CLOCK_MONOTONIC, &ts) != 0) {
            // fail TODO
        }
        msgCore->tick_stamp_ms = timespec_to_ms(&ts, &ts_start);
        pthread_mutex_lock(&msg_access_mutex);  // TODO: replace with a timed lock, or try lock?
        memset(msgPtr, &msg, th.size);
        pthread_mutex_unlock(&msg_access_mutex);
        // TODO: Implement a msg queue of a given length - subscribers may need to process a buffer
    }
};

template <typename MTopicMsgT>
class MTopicSubscriber : public MTopicBase_ {
    typedef std::function<void(const MQueuemsgT &)> callbackT;

   public:
    MTopicSubscriber(MTopicHandle_t &th, callbackT cb_) : MTopicBase_(qh, sizeof(MTopicMsgT)),
                                                          cb(cb_),
                                                          previous_tick_stamp(0),
                                                          previous_msg_count(0) {
    }

    inline uint32_t read() {
        pthread_mutex_lock(&msg_access_mutex);  //TODO: replace with a timed lock, or try lock?
        memcpy(&msg, &msgPtr, th.size);
        pthread_mutex_unlock(&msg_access_mutex);

        MsgCore_T *msgCore = static_cast<MsgCore_T *>(&msg);
        previous_tick_stamp = msgCore->tick_stamp_ms;
        if (msgCore->msg_count > previous_msg_count) {
            previous_msg_count = msgCore->msg_count;
            cb(msg);
            //TODO: define return types
            return 1;
        } else {
            return 0;
        }
    }

   private:
    MTopicMsgT msg;
    callbackT cb;
    uint32_t previous_tick_stamp;
    uint32_t previous_msg_count;
};

}  // namespace MachineRX

#endif  // _MTOPIC_HPP_