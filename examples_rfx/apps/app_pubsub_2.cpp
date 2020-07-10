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
 * File: app_pubsub_2.cpp
 * Project: MachineRX
 * Author: Ali AlSaibie (ali.alsaibie@ku.edu.kw)
 * -----
 * Modified By: Ali AlSaibie (ali.alsaibie@ku.edu.kw>)
 */

#include "MachineRFX.hpp"
#include "app_starter.hpp"
#include "topic1.hpp"
#include "topic2.hpp"

using namespace MachineRFX;
class AppPubSub2 : public MRXThread {
   public:
    AppPubSub2() : MRXThread("PubSub 2", 512, MRXPriority_n::Normal, 500),
                   topic_2_pub(gTopic2MTHandle),
                   topic_1_sub(gTopic1MTHandle,
                               std::bind(&AppPubSub2::on_topic_1_read, this, std::placeholders::_1)) {
        //  printf("Hi\n");
    }
    virtual ~AppPubSub2() {
    }

   protected:
    virtual void run() {
        printf("Thread Priority: %d\n", (int)getPriority());
        printf("Thread Name: ");
        printf("%s", getName());
        printf("\n");

        while (1) {
            topic2Msg.P.a = 1;

            topic_2_pub.publish(topic2Msg);

            topic_1_sub.read();

            thread_lap();
        }
    }

   private:
    void on_topic_1_read(const Topic1_msg_t &msg) {
        topic1Msg = msg;
        printf("App 2 Received Message - Count: %i, Time(ms): %i\n", topic1Msg.msg_count, topic1Msg.tick_stamp);
    }

    /* Topic Pubs */
    MRXTopicPublisher<Topic2_msg_t> topic_2_pub;
    Topic2_msg_t topic2Msg;
    /* Topic Subs */

    MRXTopicSubscriber<Topic1_msg_t> topic_1_sub;
    Topic1_msg_t topic1Msg;
};

void start_application_pubsub_2() {
    AppPubSub2 *ptr = new AppPubSub2();
    ptr->start();
}