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
 * File: main_app.cpp
 * Project: MachineRX
 * Author: Ali AlSaibie (ali.alsaibie@ku.edu.kw)
 * -----
 * Modified By: Ali AlSaibie (ali.alsaibie@ku.edu.kw>)
 */

#include "main_app.hpp"

#include "MachineRX.hpp"
#include "topic1.hpp"
#include "FreeRTOS.h"
#include "task.h"

bool applications_started = false;
void* operator new( size_t size )
{
    if (xTaskGetSchedulerState() == 1)
    {
        //what should I do???????
    }
    return pvPortMalloc( size );
}

void* operator new[]( size_t size )
{
    return pvPortMalloc(size);
}

void operator delete( void * ptr )
{
    vPortFree ( ptr );
}

void operator delete[]( void * ptr )
{
    vPortFree ( ptr );
}
int main_app(int argc, char **argv) {
    //TODO: Add assert guards
    size_t heapfree = xPortGetFreeHeapSize();
    MachineRX::initialize_start_timespec();
    MachineRX::msleep(1);

    // start_application_0();
    start_application_1();
    // start_application_multi_thread_mutex_printf();
    // start_application_test_FreeRTOS_POSIX_mutex();
    // start_application_test_FreeRTOS_mutex();

    heapfree = xPortGetFreeHeapSize();

    // start_application_2();
}