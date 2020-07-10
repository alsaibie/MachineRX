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
 * File: MRXTopic.hpp
 * Project: MachineRX
 * Author: Ali AlSaibie (ali.alsaibie@ku.edu.kw)
 * -----
 * Modified By: Ali AlSaibie (ali.alsaibie@ku.edu.kw>)
 */

#ifndef _MRFXTopic_HPP_
#define _MRFXTopic_HPP_

#include <cassert>
#include <vector>
#include <functional>
#include "MRXUtil.hpp"
#include "stm32f4xx_hal.h"
namespace MachineRFX {

struct _msgCore {
    uint32_t tick_stamp{0};
    uint32_t msg_count{0};
};

typedef struct {
    const char *name;
    uint32_t length{1};
    QueueHandle_t handle{NULL};
} MRXTopicHandle_t;

class MRXTopicBase_ {
   public:
    inline const char *getName() {
        return th.name;
    }

    inline uint32_t getLength() {
        return th.length;
    }

    inline QueueHandle_t getHandle() {
        return th.handle;
    }

   protected:
    MRXTopicBase_(MRXTopicHandle_t &th_, size_t msg_size_) : th(th_), msg_size(msg_size_) {
        if (th.handle == NULL) {
            /* Create Queue if not created */
            th.handle = xQueueCreate(th.length, msg_size);
            ptrTH.push_back(&th);
        }
    }
    ~MRXTopicBase_() {
        //TODO: pop out of vector ptrMTH
        //TODO: destroy queue
    }
    MRXTopicHandle_t &th;

   private:
    inline static std::vector<MRXTopicHandle_t *> ptrTH;
    uint32_t msg_size{0};
};

template <typename MRXTopicMsgT>
class MRXTopicPublisher : public MRXTopicBase_ {
   public:
    MRXTopicPublisher(MRXTopicHandle_t &th) : MRXTopicBase_(th, sizeof(MRXTopicMsgT)) {
    }

    inline uint32_t publish(MRXTopicMsgT &msg) {
        _msgCore *msgCore = static_cast<_msgCore *>(&msg);
        msgCore->msg_count++;
        msgCore->tick_stamp = HAL_GetTick();
        if (xQueueOverwrite(th.handle, &msg) == pdTRUE) {  // TODO: create a seperate multi-publisher object
            /* Save a copy for peeking */
            return pdPASS;
        } else {
            return pdFAIL;
        }
        // TODO: Have the the receiver accumulate data if needed and then process on demand.
    }

   private:
    MRXTopicMsgT msg;
};

template<typename MRXTopicMsgT>
class MRXTopicSubscriber : public MRXTopicBase_ {
  typedef std::function<void(const MRXTopicMsgT&)> callbackT;
 public:
  MRXTopicSubscriber(MRXTopicHandle_t &th_, callbackT cb_) :
      MRXTopicBase_(th_, sizeof(MRXTopicMsgT)),
      cb(cb_),
      previous_tick_stamp(0),
      previous_msg_count(0) {
  }

  inline uint32_t read() {
    if (xQueuePeek(th.handle, &msg, (TickType_t) 10) == pdPASS) {
      _msgCore *msgCore = static_cast<_msgCore*>(&msg);
      previous_tick_stamp = msgCore->tick_stamp;
      if (msgCore->msg_count > previous_msg_count) {
        previous_msg_count = msgCore->msg_count;
        cb(msg);
        return pdPASS;
      } else {
        return pdFAIL;
      }
    } else {
      return pdFAIL; //TODO: distinguish between failed read and old msg
    }
  }

 private:
  MRXTopicMsgT msg;
  callbackT cb;
  uint32_t previous_tick_stamp;
  uint32_t previous_msg_count;
};

}  // namespace MachineRFX

#endif  // _MRFXTopic_HPP_