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
 * File: app_test_FreeRTOS_POSIX_mutex.cpp
 * Project: MachineRX
 * Author: Ali AlSaibie (ali.alsaibie@ku.edu.kw)
 * -----
 * Modified By: Ali AlSaibie (ali.alsaibie@ku.edu.kw>)
 */

#include <assert.h>
#include <pthread.h>

#include "stdint.h"
#include "stdio.h"

pthread_mutex_t printf_mutex;

void msleep(uint32_t ms) {
    struct timespec ts_;
    ts_.tv_sec = ms / 1000L;
    ts_.tv_nsec = (ms % 1000L) * 1000000L;
    nanosleep(&ts_, NULL);
}

void *printf_posix_task(void *arg) {
    static int count = 0;
    int taskno = (int *)arg;
    while (1) {    
        struct timespec timeoutTime;
        clock_gettime(CLOCK_MONOTONIC, &timeoutTime);
        timeoutTime.tv_nsec += 10 * 1000000L;
        if (pthread_mutex_timedlock(&printf_mutex, &timeoutTime) == 0) {
            printf("Hi from Task %d, count %d\n", (int *)arg, count++);
            pthread_mutex_unlock(&printf_mutex);
        };

        msleep(1000);
    }
}

#define SCHED_RR 2
#define SCHED_FIFO 1

void start_application_test_FreeRTOS_POSIX_mutex() {
    pthread_t tasks_test_freertos[10];
    int task_num[10];
    for (int k = 0; k < 10; k++) {
        task_num[k] = k;
        pthread_attr_t thread_attr;
        uint32_t ret;
        struct sched_param schparam;

        ret = pthread_attr_init(&thread_attr);
        assert(ret == 0);

        ret = pthread_attr_getschedparam(&thread_attr, &schparam);
        assert(ret == 0);
        schparam.sched_priority = 2;

        /* Round-Robin Scheduling Policy */
        ret = pthread_attr_setschedpolicy(&thread_attr, SCHED_FIFO);
        assert(ret == 0);

        ret = pthread_attr_setschedparam(&thread_attr, &schparam);
        assert(ret == 0);

        ret = pthread_create(&tasks_test_freertos[k], &thread_attr, printf_posix_task, task_num[k]);
        assert(ret == 0);
    }
}