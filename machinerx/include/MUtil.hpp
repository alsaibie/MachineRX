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
 * File: MUtil.cpp
 * Project: MachineRX
 * Author: Ali AlSaibie (ali.alsaibie@ku.edu.kw)
 * -----
 * Modified By: Ali AlSaibie (ali.alsaibie@ku.edu.kw>)
 */

#ifndef _MUTIL_HPP_
#define _MUTIL_HPP_

//TODO: move time management functions here
//TODO:



extern "C" {
// #include "FreeRTOS_POSIX/pthread.h"
// #include "FreeRTOS.h"
#include <pthread.h>
#include <assert.h>
// #include <malloc.h>
// #include "pthread.h"
#include "mxr_allocators.h"
}

#include <cstdint>

// Some defines for compatability
#ifndef SCHED_RR
    #define SCHED_RR 2
#endif 

#ifndef PTHREAD_STACK_MIN
    #define PTHREAD_STACK_MIN 512
#endif

namespace MachineRX {

inline timespec gts_start{0, 0};

inline uint32_t initialize_start_timespec() {
    clock_gettime(CLOCK_MONOTONIC, &gts_start);
}

inline uint32_t uptime_ms() {
    timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    uint32_t t_ms = (ts.tv_sec - gts_start.tv_sec) * 1000L;
    int32_t delta_tms = (ts.tv_nsec / 1000000L - gts_start.tv_nsec / 1000000L);
    if (delta_tms >= 0) {
        t_ms += delta_tms;
    } else {
        t_ms += 1000 + delta_tms;
    }
    return t_ms;
}

inline uint32_t timespec_to_ms(struct timespec &ts) {
    uint32_t t_ms = ts.tv_sec * 1000L;
    t_ms += ts.tv_nsec / 1000000L;
    return t_ms;
}


}  // namespace MachineRX

#endif