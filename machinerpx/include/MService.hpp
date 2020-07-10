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
 * File: MService.hpp
 * Project: MachineRX
 * Author: Ali AlSaibie (ali.alsaibie@ku.edu.kw)
 * -----
 * Modified By: Ali AlSaibie (ali.alsaibie@ku.edu.kw>)
 */
#ifndef _MSERVICE_HPP_
#define _MSERVICE_HPP_

#include <cstdint>
#include <cstdio>
#include <cstring>

#ifdef __cpluplus
extern "C" {
#endif

#include <assert.h>
// #include <malloc.h>

#ifdef __cpluplus
}
#endif

#include <functional>
#include <vector>
#include "MUtil.hpp"


namespace MachineRPX {

inline static pthread_mutex_t gservice_initialization_mutex;
inline void initialize_service_mutex(void) {
    pthread_mutex_init(&gservice_initialization_mutex, NULL);
}

typedef uint32_t sd_t;

struct _srvCore {
    uint32_t tick_stamp_ms{0};
    uint32_t srv_count{0};
};

typedef struct {
    const char *name;
    const uint32_t length;
    sd_t id{'\0'};
    pthread_mutex_t srv_access_mutex{};
    void *srvPtr{NULL};
} MServiceHandle_t;

//TODO: Consider ROS's implementation first, this will just be a way to forward service messages to and from the ROS interface

} // namespace MachineRPX

#endif // _MSERVICE_HPP_