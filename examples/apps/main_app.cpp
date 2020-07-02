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
#include <stdio.h>
#include "MachineRX.hpp"

using namespace MachineRX;

class TestManager : public MThread
{
public:
    TestManager() : MThread("Test Manager", 512, 2, 1000)
    {
    }
    virtual ~TestManager()
    {
    }

protected:
    virtual void run()
    {

        printf("Thread Priority: %d\n", getThreadPriority());
        printf("Thread Name: ");
        printf(getThreadName());
        printf("\n");

        while (1)
        {
            struct timespec begin, end;
            double elapsed;
            clock_gettime(CLOCK_MONOTONIC, &begin);

            msleep(2);
            thread_lap();

            clock_gettime(CLOCK_MONOTONIC, &end);
            elapsed = end.tv_sec - begin.tv_sec;
            elapsed += (end.tv_nsec - begin.tv_nsec) / 1000000000.0L;
            printf("ET %fms\n", elapsed * 1000L);
        }
    }
};

void main_app(void)
{
    TestManager *ptr = new TestManager();
    ptr->start();
    printf("Hi\n");
}