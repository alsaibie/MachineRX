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

#include "stm32f4xx_hal.h"

namespace MachineRFX {

enum class MRXPriority_n{
    Idle = 1,              /* idle (lowest) */

    Low  = 8,              /* (low) */
    Low1 = 8+1,            /* (low + 1) */
    Low2 = 9+1,            /* (low + 2) */
    Low3 = 8+3,            /* (low + 3) */
    Low4 = 8+4,            /* (low + 4) */
    Low5 = 8+5,            /* (low + 5) */
    Low6 = 8+6,            /* (low + 6) */
    Low7 = 8+7,            /* (low + 7) */

    BelowNormal  = 16,     /* (Below Normal) */
    BelowNormal1 = 16+1,   /* (Below Normal + 1) */
    BelowNormal2 = 16+1,   /* (Below Normal + 2) */
    BelowNormal3 = 16+3,   /* (Below Normal + 3) */
    BelowNormal4 = 16+4,   /* (Below Normal + 4) */
    BelowNormal5 = 16+5,   /* (Below Normal + 5) */
    BelowNormal6 = 16+6,   /* (Below Normal + 6) */
    BelowNormal7 = 16+7,   /* (Below Normal + 7) */
    
    Normal  = 24,          /* (Normal) */
    Normal1 = 24+1,        /* (Normal + 1) */
    Normal2 = 24+1,        /* (Normal + 2) */
    Normal3 = 24+3,        /* (Normal + 3) */
    Normal4 = 24+4,        /* (Normal + 4) */
    Normal5 = 24+5,        /* (Normal + 5) */
    Normal6 = 24+6,        /* (Normal + 6) */
    Normal7 = 24+7,        /* (Normal + 7) */

    AboveNormal  = 32,     /* (Above Normal) */
    AboveNormal1 = 32+1,   /* (Above Normal + 1) */
    AboveNormal2 = 32+1,   /* (Above Normal + 2) */
    AboveNormal3 = 32+3,   /* (Above Normal + 3) */
    AboveNormal4 = 32+4,   /* (Above Normal + 4) */
    AboveNormal5 = 32+5,   /* (Above Normal + 5) */
    AboveNormal6 = 32+6,   /* (Above Normal + 6) */
    AboveNormal7 = 32+7,   /* (Above Normal + 7) */

    High  = 40,            /* (High) */
    High1 = 40+1,          /* (High + 1) */
    High2 = 40+1,          /* (High + 2) */
    High3 = 40+3,          /* (High + 3) */
    High4 = 40+4,          /* (High + 4) */
    High5 = 40+5,          /* (High + 5) */
    High6 = 40+6,          /* (High + 6) */
    High7 = 40+7,          /* (High + 7) */

    Realtime  = 48,        /* (Realtime) */
    Realtime1 = 48+1,      /* (Realtime + 1) */
    Realtime2 = 48+1,      /* (Realtime + 2) */
    Realtime3 = 48+3,      /* (Realtime + 3) */
    Realtime4 = 48+4,      /* (Realtime + 4) */
    Realtime5 = 48+5,      /* (Realtime + 5) */
    Realtime6 = 48+6,      /* (Realtime + 6) */
    Realtime7 = 48+7,      /* (Realtime + 7) */

    RealTime = +3,         /* realtime (highest) */

    ISR = 56,              /* reserved for ISR */

    Error = -1             /* system cannot determine priority or thread has illegal priority */
};

inline void uSleep(uint32_t t_us) {

}

inline void toggleLD2(){
    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_14);
}
inline void toggleLD3(){
    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_7);
}

}  // namespace MachineRFX

#endif  // _MRFXUtil_HPP_