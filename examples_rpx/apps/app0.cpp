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
 * File: app0.cpp
 * Project: MachineRX
 * Author: Ali AlSaibie (ali.alsaibie@ku.edu.kw)
 * -----
 * Modified By: Ali AlSaibie (ali.alsaibie@ku.edu.kw>)
 */

#include "MachineRPX.hpp"
#include "main_app.hpp"
// #include "topic1.hpp"
// #include "topic2.hpp"

using namespace MachineRPX;

class Application0 : public MThread {
   public:
    Application0() : MThread("Application 0", 512 * 4, 2, 1000) {
    }
    virtual ~Application0() {
    }

   protected:
    virtual void run() {
        printf("Thread Priority: %d\n", getThreadPriority());
        printf("Thread Name: ");
        printf("%s", getThreadName());
        printf("\n");

        while (1) {
            struct timespec begin, end;
            double elapsed;
            clock_gettime(CLOCK_MONOTONIC, &begin);

            printf("Hi From App 0\n");

            thread_lap();

            clock_gettime(CLOCK_MONOTONIC, &end);
            elapsed = end.tv_sec - begin.tv_sec;
            elapsed += (end.tv_nsec - begin.tv_nsec) / 1000000000.0L;
            // printf("ET %0.2f ms\n", elapsed * 1000L);
            printf("ET %i us\n", int(elapsed * 1000000L));
        }
    }

   private:

};
Application0 app0;
void start_application_0() {
    // Application0 *ptr = new Application0();
    // ptr->start();
    app0.start();
}