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
 * File: app_multi_pub_single_sub_example.cpp
 * Project: MachineRX
 * Author: Ali AlSaibie (ali.alsaibie@ku.edu.kw)
 * -----
 * Modified By: Ali AlSaibie (ali.alsaibie@ku.edu.kw>)
 * * A single subscriber waits for new messages on all topics before processing information/
 * To illustrate the example, multiple publishers of multiple topics are spawned.
 * The subscriber keeps a list of of topics to align and checks that 
 */

#include <string>

#include "MachineRFX.hpp"
#include "app_starter.hpp"
#include "stm32f4xx_hal.h"
#include "topic1.hpp"
#include "topic2.hpp"
#include "topic3.hpp"
#include "topic4.hpp"
#include "topic5.hpp"

using namespace MachineRFX;

/* Publisher */
template <typename TopicMsgT>
class PublisherApp : public MRXThread {
   public:
    PublisherApp(uint32_t num_, MRXTopicHandle_t &topic_handle_) : num(num_),
                                                                   MRXThread("Publisher", 256, MRXPriority_n::RealTime, 100),
                                                                   topic_pub(topic_handle_) {
    }
    virtual ~PublisherApp() {
    }

   protected:
    virtual void run() {
        while (1) {
            topicMsg.P.a = num;
            topic_pub.publish(topicMsg);
            thread_lap();
        }
    }

   private:
    /* Topic Pubs */
    uint32_t num;
    MRXTopicPublisher<TopicMsgT> topic_pub;
    TopicMsgT topicMsg;
};

class SubscriberApp : public MRXThread {
   public:
    SubscriberApp() : MRXThread("Subscriber", 512, MRXPriority_n::RealTime, 1),
                      topic_1_sub(gTopic1MTHandle, std::bind(&SubscriberApp::on_topic_1_read, this, std::placeholders::_1)),
                      topic_2_sub(gTopic2MTHandle, std::bind(&SubscriberApp::on_topic_2_read, this, std::placeholders::_1)),
                      topic_3_sub(gTopic3MTHandle, std::bind(&SubscriberApp::on_topic_3_read, this, std::placeholders::_1)),
                      topic_4_sub(gTopic4MTHandle, std::bind(&SubscriberApp::on_topic_4_read, this, std::placeholders::_1)),
                      topic_5_sub(gTopic5MTHandle, std::bind(&SubscriberApp::on_topic_5_read, this, std::placeholders::_1)) {
    }
    virtual ~SubscriberApp() {
    }

   protected:
    virtual void run() {
        /* This is one of many tasks that wait for a new specific shared topic */
        while (1) {
            auto new1 = topic_1_sub.read();
            auto new2 = topic_2_sub.read();
            auto new3 = topic_3_sub.read();
            auto new4 = topic_4_sub.read();
            auto new5 = topic_5_sub.read();

            /* Zero-wait task yield - a reschedule only */
            vTaskDelay(0);
            //TODO: do some mathematical processes
            // thread_lap(); /* Thread doesn't lap, only waits on new data */
        }
    }

   private:
    void on_topic_1_read(const Topic1_msg_t &msg) {
        topic1Msg = msg;
        uint8_t latency = HAL_GetTick() - topic1Msg.tick_stamp;
        printf("S%i#%iL%ims\n", topic1Msg.P.a, topic1Msg.msg_count, latency);
    }
    void on_topic_2_read(const Topic2_msg_t &msg) {
        topic2Msg = msg;
        uint8_t latency = HAL_GetTick() - topic2Msg.tick_stamp;
        printf("S%i#%iL%ims\n", topic2Msg.P.a, topic2Msg.msg_count, latency);
    }
    void on_topic_3_read(const Topic3_msg_t &msg) {
        topic3Msg = msg;
        uint8_t latency = HAL_GetTick() - topic3Msg.tick_stamp;
        printf("S%i#%iL%ims\n", topic3Msg.P.a, topic3Msg.msg_count, latency);
    }
    void on_topic_4_read(const Topic4_msg_t &msg) {
        topic4Msg = msg;
        uint8_t latency = HAL_GetTick() - topic4Msg.tick_stamp;
        printf("S%i#%iL%ims\n", topic4Msg.P.a, topic4Msg.msg_count, latency);
    }
    void on_topic_5_read(const Topic5_msg_t &msg) {
        topic5Msg = msg;
        uint8_t latency = HAL_GetTick() - topic5Msg.tick_stamp;
        printf("S%i#%iL%ims\n", topic5Msg.P.a, topic5Msg.msg_count, latency);
    }

    /* Topic Subs */
    MRXTopicSubscriber<Topic1_msg_t> topic_1_sub;
    MRXTopicSubscriber<Topic2_msg_t> topic_2_sub;
    MRXTopicSubscriber<Topic3_msg_t> topic_3_sub;
    MRXTopicSubscriber<Topic4_msg_t> topic_4_sub;
    MRXTopicSubscriber<Topic5_msg_t> topic_5_sub;

    Topic1_msg_t topic1Msg;
    Topic2_msg_t topic2Msg;
    Topic3_msg_t topic3Msg;
    Topic4_msg_t topic4Msg;
    Topic5_msg_t topic5Msg;
};

void start_application_multi_pub_single_sub_example() {
    PublisherApp<Topic1_msg_t> *ptr1 = new PublisherApp<Topic1_msg_t>(1, gTopic1MTHandle);
    PublisherApp<Topic2_msg_t> *ptr2 = new PublisherApp<Topic2_msg_t>(2, gTopic2MTHandle);
    PublisherApp<Topic3_msg_t> *ptr3 = new PublisherApp<Topic3_msg_t>(3, gTopic3MTHandle);
    PublisherApp<Topic4_msg_t> *ptr4 = new PublisherApp<Topic4_msg_t>(4, gTopic4MTHandle);
    PublisherApp<Topic5_msg_t> *ptr5 = new PublisherApp<Topic5_msg_t>(5, gTopic5MTHandle);

    ptr1->start();
    ptr2->start();
    ptr3->start();
    ptr4->start();
    ptr5->start();

    SubscriberApp *ptrSub = new SubscriberApp();
    ptrSub->start();
}
