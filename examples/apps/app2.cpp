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
 * File: app2.cpp
 * Project: MachineRX
 * Author: Ali AlSaibie (ali.alsaibie@ku.edu.kw)
 * -----
 * Modified By: Ali AlSaibie (ali.alsaibie@ku.edu.kw>)
 */
#include "MachineRX.hpp"
#include "main_app.hpp"
#include "topic1.hpp"
#include "topic2.hpp"

using namespace MachineRX;

class Application2 : public MThread {
   public:
    Application2() : MThread("Application 2", 512 * 4, 2, 1000),
                     topic_2_pub(gTopic2MTHandle),
                     topic_1_sub(gTopic1MTHandle,
                                 std::bind(&Application2::on_topic_1_read, this, std::placeholders::_1)) {
    }
    virtual ~Application2() {
    }

   protected:
    virtual void run() {
        topic_1_sub.initialize();
        topic_2_pub.initialize();
        printf("Thread Priority: %d\n", getThreadPriority());
        printf("Thread Name: ");
        printf("%s", getThreadName());
        printf("\n");

        while (1) {
            // struct timespec begin, end; //TODO: move time example to separate app
            // double elapsed;
            // clock_gettime(CLOCK_MONOTONIC, &begin);

            
            topic2Msg.P.a = 2;
            
            topic_2_pub.publish(topic2Msg);
            
            topic_1_sub.read();


            thread_lap();


            // clock_gettime(CLOCK_MONOTONIC, &end);
            // elapsed = end.tv_sec - begin.tv_sec;
            // elapsed += (end.tv_nsec - begin.tv_nsec) / 1000000000.0L;
            // printf("ET %fms\n", elapsed * 1000L);
        }
    }

   private:
    void on_topic_1_read(const Topic1_msg_t &msg) {
        topic1Msg = msg;
        printf("App 2 Received Message - Count: %i, Time(ms): %i\n", topic1Msg.msg_count, topic1Msg.tick_stamp_ms);
    }

    /* Pubs */
    MachineRX::MTopicPublisher<Topic2_msg_t> topic_2_pub;
    Topic2_msg_t topic2Msg;
    /* Subs */
    MachineRX::MTopicSubscriber<Topic1_msg_t> topic_1_sub;
    Topic1_msg_t topic1Msg;
};

Application2 app2;
void start_application_2() {
    // Application2 *ptr = new Application2();
    // ptr->start();
    app2.start();
}