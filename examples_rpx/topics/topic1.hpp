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
 * File: topic1.hpp
 * Project: MachineRX
 * Author: Ali AlSaibie (ali.alsaibie@ku.edu.kw)
 * -----
 * Modified By: Ali AlSaibie (ali.alsaibie@ku.edu.kw>)
 */
#ifndef _TOPIC1_HPP_
#define _TOPIC1_HPP_

#include "MTopic.hpp"
namespace MachineRPX {
typedef struct : public _msgCore {
    struct {
        int a{0};
        // char buf[20]{};
        // float f{};
    } P;

} Topic1_msg_t;

// inline pthread_mutex_t gTopic1_access_mutex{0};

inline MachineRPX::MTopicHandle_t gTopic1MTHandle{"Topic 1", 1}; /* The rest NULLs */
}  // namespace MachineRPX

#endif /* _TOPIC1_HPP_ */