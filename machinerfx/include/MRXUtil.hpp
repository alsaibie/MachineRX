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
 * File: MRXUtil.hpp
 * Project: MachineRX
 * Author: Ali AlSaibie (ali.alsaibie@ku.edu.kw)
 * -----
 * Modified By: Ali AlSaibie (ali.alsaibie@ku.edu.kw>)
 */

#ifndef _MRFXUtil_HPP_
#define _MRFXUtil_HPP_

#ifdef __cplusplus
extern "C" {
#endif
#include <assert.h>

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "mrfx_allocators.h"
#ifdef __cplusplus
}
#endif

namespace MachineRFX {

enum class MRXPriority_n{
    Idle = -3,        /* idle (lowest) */
    Low = -2,         /* (low) */
    BelowNormal = -1, /* below normal */
    Normal = 0,       /* normal (default) */
    AboveNormal = +1, /* above normal */
    High = +2,        /* high */
    RealTime = +3,    /* realtime (highest) */
    Error = 0x84      /* system cannot determine priority or thread has illegal priority */
};

inline void uSleep(uint32_t t_us) {

}

}  // namespace MachineRFX

#endif  // _MRFXUtil_HPP_