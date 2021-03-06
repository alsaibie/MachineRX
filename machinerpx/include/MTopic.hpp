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
 * Project: MachineRX
 * Author: Ali AlSaibie (ali.alsaibie@ku.edu.kw)
 * -----
 * Modified By: Ali AlSaibie (ali.alsaibie@ku.edu.kw>)
 */

#ifndef _MTOPIC_HPP_
#define _MTOPIC_HPP_
#include <cstdint>
#include <cstdio>
#include <cstring>
#include "MUtil.hpp"
#include <cassert>
#include <functional>
#include <vector>

//TODO: Add assert guards
namespace MachineRPX {

// inline static pthread_mutex_t gtopic_initialization_mutex;
inline pthread_mutex_t gtopic_initialization_mutex{0};

typedef uint32_t td_t;

struct _msgCore {
    uint32_t tick_stamp_ms{0};
    uint32_t msg_count{0};
};

typedef struct {
    const char *name;
    const uint32_t length;
    pthread_mutex_t * msg_access_mutex{NULL};
    td_t id{'\0'};
    void *msgPtr{NULL};
} MTopicHandle_t;

class MTopicBase_ {
   public:
    inline const char *getName() {
        return th.name;
    }

    inline const td_t getId() {
        return th.id;
    }

    inline const uint32_t getLength() {
        return th.length;
    }

    /* Requires C++17 */
    // inline static std::vector<MTopicHandle_t *> ptrMTH;
    inline static bool time_spec_offset{false};
    inline static uint32_t id_counter{0};

    void initialize(void) {
        if (th.id == '\0') {
            struct timespec timeoutTime;
            clock_gettime(CLOCK_MONOTONIC, &timeoutTime);
            timeoutTime.tv_sec += 1;
            if (pthread_mutex_timedlock(&gtopic_initialization_mutex, &timeoutTime) == 0) {
                /* Check again in case it was initialized while waiting for mutex lock */
                if (th.id == '\0') {
                    th.id = id_counter + 1;
                    id_counter++;
                    th.msg_access_mutex = new pthread_mutex_t;
                    pthread_mutex_init(th.msg_access_mutex, NULL);
                    th.msgPtr = create_shared_memspace(msgSize);
                    // ptrMTH.push_back(&th);
                }
                pthread_mutex_unlock(&gtopic_initialization_mutex);

            } else {
                printf("Can't initialize Topic - Timeout\n");
            }
        }
    }

   protected:
    MTopicBase_(MTopicHandle_t &th_, uint32_t msgSize_) : th(th_), msgSize(msgSize_) {
    }

    ~MTopicBase_() {
        //TODO: pop out of vector ptrMTH
        //TODO: release mutex
        //TODO:
        destroy_shared_memspace();
    }

    MTopicHandle_t &th;
    uint32_t msgSize;

   private:
    inline void *create_shared_memspace(uint32_t size) {
        void *ptr = mrpx_allocator.malloc(size);  //TODO: replace with custom per platform allocate
        memset(ptr, 0, size);
        return ptr;
    }

    inline void destroy_shared_memspace() {
        mrpx_allocator.free(th.msgPtr);  //TODO: replace with custom per platform dealloc
    }
};

template <typename MTopicMsgT>
class MTopicPublisher : public MTopicBase_ {
   public:
    MTopicPublisher(MTopicHandle_t &th) : MTopicBase_(th, sizeof(MTopicMsgT)) {
    }

    inline uint32_t publish(MTopicMsgT &msg) {
        _msgCore *msgCore = static_cast<_msgCore *>(&msg);
        msgCore->msg_count++;
        msgCore->tick_stamp_ms = uptime_ms();
        struct timespec timeoutTime;
        clock_gettime(CLOCK_MONOTONIC, &timeoutTime);
        timeoutTime.tv_nsec += 10 * 1000000L;
        if (pthread_mutex_timedlock(th.msg_access_mutex, &timeoutTime) == 0) {
            (void)memcpy((void *)th.msgPtr, &msg, (size_t)msgSize);
            pthread_mutex_unlock(th.msg_access_mutex);
        }

        // TODO: Implement a msg queue of a given length - subscribers may need to process a buffer
        // TODO: Alternatively, have the the receiver accumulate data if needed and then process on demand.
    }
};

template <typename MTopicMsgT>
class MTopicSubscriber : public MTopicBase_ {
    typedef std::function<void(const MTopicMsgT &)> callbackT;

   public:
    MTopicSubscriber(MTopicHandle_t &th, callbackT cb_) : MTopicBase_(th, sizeof(MTopicMsgT)),
                                                          cb(cb_) {
    }

    inline uint32_t read() {
        struct timespec timeoutTime;
        clock_gettime(CLOCK_MONOTONIC, &timeoutTime);
        timeoutTime.tv_nsec += 10 * 1000000L;
        if (pthread_mutex_timedlock(th.msg_access_mutex, &timeoutTime) == 0) {
            (void)memcpy((void *)&msg, (void *)th.msgPtr, (size_t)msgSize);
            pthread_mutex_unlock(th.msg_access_mutex);
            _msgCore *msgCore = static_cast<_msgCore *>(&msg);
            previous_tick_stamp = msgCore->tick_stamp_ms;
            if (msgCore->msg_count > previous_msg_count) {
                previous_msg_count = msgCore->msg_count;
                cb(msg);
                //TODO: define error return types, any standard posix?
                return 1;
            } else {
                return 0;
            }
        }
    }

   private:
    MTopicMsgT msg;
    callbackT cb;
    uint32_t previous_tick_stamp{0};
    uint32_t previous_msg_count{0};
};

}  // namespace MachineRPX

#endif  // _MTOPIC_HPP_