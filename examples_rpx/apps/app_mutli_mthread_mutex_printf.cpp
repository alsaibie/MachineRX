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
 * File: app_mutli_mthread_mutex_printf.cpp
 * Project: MachineRX
 * Author: Ali AlSaibie (ali.alsaibie@ku.edu.kw)
 * -----
 * Modified By: Ali AlSaibie (ali.alsaibie@ku.edu.kw>)
 */

#include <MThread.hpp>
#include <cstdio>
#include <vector>

pthread_mutex_t multithread_printf_mutex;

using namespace MachineRPX;

class MutliMThread : public MThread {
   public:
    MutliMThread(int thread_no_) : MThread("Multi Thread 1", 256 * 4, 2, 100), thread_no(thread_no_) {
    }
    virtual ~MutliMThread() {
    }

   protected:
    virtual void run() {
        while (1) {
            struct timespec timeoutTime;
            clock_gettime(CLOCK_MONOTONIC, &timeoutTime);
            timeoutTime.tv_nsec += 10 * 1000000L;
            if (pthread_mutex_timedlock(&multithread_printf_mutex, &timeoutTime) == 0) {
                printf("Hi from Thread %d, count %d\n", thread_no, count++);
                pthread_mutex_unlock(&multithread_printf_mutex);
            };

            thread_lap();
        }
    }

   private:
    int thread_no;
    inline static int count{0};
};

void start_application_multi_thread_mutex_printf() {
    std::vector<MutliMThread *> ptrThreads;
    for (auto k = 0; k < 10; k++) {
        MutliMThread * ptr = new MutliMThread(k);
        ptr->start();
        ptrThreads.push_back(ptr);
    }
}