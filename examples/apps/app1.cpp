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
 * File: app1.cpp
 * Project: MachineRX
 * Author: Ali AlSaibie (ali.alsaibie@ku.edu.kw)
 * -----
 * Modified By: Ali AlSaibie (ali.alsaibie@ku.edu.kw>)
 */

#include "MachineRX.hpp"
#include "topic1.hpp"
#include "main_app.hpp"

#include "topic2.hpp"

using namespace MachineRX;
class Application1 : public MThread {
   public:
    Application1() : MThread("Application 1", 512 * 4, 20, 1000),
                     topic_1_pub(gTopic1MTHandle) {
        //  topic_2_sub(gTopic2MTHandle,
        //              std::bind(&Application1::on_topic_2_read, this, std::placeholders::_1)) {
    }
    virtual ~Application1() {
    }

   protected:
    virtual void run() {
        topic_1_pub.initialize();
        // topic_2_sub.initialize();

        printf("Thread Priority: %d\n", getThreadPriority());
        printf("Thread Name: ");
        printf("%s", getThreadName());
        printf("\n");

        while (1) {
            struct timespec begin, end;
            double elapsed;
            clock_gettime(CLOCK_MONOTONIC, &begin);

            // msleep(2);

            topic1Msg.P.a = 1;

            topic_1_pub.publish(topic1Msg);

            // topic_2_sub.read();

            thread_lap();

            clock_gettime(CLOCK_MONOTONIC, &end);
            elapsed = end.tv_sec - begin.tv_sec;
            elapsed += (end.tv_nsec - begin.tv_nsec) / 1000000000.0L;
            printf("ET %i us\n", int(elapsed * 1000000L));
        }
    }

   private:
    // void on_topic_2_read(const Topic2_msg_t &msg) {
    //     topic2Msg = msg;
    //     printf("App 1 Received Message - Count: %i, Time(ms): %i\n", topic2Msg.msg_count, topic2Msg.tick_stamp_ms);
    // }

    /* Topic Pubs */
    MachineRX::MTopicPublisher<Topic1_msg_t> topic_1_pub;
    Topic1_msg_t topic1Msg;
    /* Topic Subs */
    // MachineRX::MTopicSubscriber<Topic2_msg_t> topic_2_sub;
    // Topic2_msg_t topic2Msg;
};


void start_application_1() {
    Application1 *ptr = new Application1();
    ptr->start();
}