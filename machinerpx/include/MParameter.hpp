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
 * File: MParameter.hpp
 * Project: MachineRX
 * Author: Ali AlSaibie (ali.alsaibie@ku.edu.kw)
 * -----
 * Modified By: Ali AlSaibie (ali.alsaibie@ku.edu.kw>)
 */

#ifndef _PARAMETER_HPP_
#define _PARAMETER_HPP_

#include <cstdint>
#include <cstdio>
#include <cstring>

#ifdef __cpluplus
extern "C" {
#endif
#include <assert.h>
// #include <malloc.h>
// #include <pthread.h>

#ifdef __cpluplus
}
#endif

#include <functional>
#include <vector>
#include "MUtil.hpp"

/*
*   Hashing notes:
*   
*   Concept: Usually a paramater is set from a user interface (ROS/PC Comms) and directly to a thread/block
*   
*   Each block/thread registers its parameters as objects, they are saved in a list. 
*   The setter knows the parameter object type, and the target. from setter: set(target, parameters)
*   
*   The receiver checks periodically for new parameters
*   The External Interface can also advertise the list of parameters/threads
*   
* */

namespace MachineRPX {

inline static pthread_mutex_t gparameter_initialization_mutex;
inline void initialize_parameter_mutex(void) {
    pthread_mutex_init(&gparameter_initialization_mutex, NULL);
}

typedef uint32_t pd_t;

struct _prmCore {
    uint32_t tick_stamp_ms{0};
    uint32_t prm_count{0};
};

typedef struct {
    const char *name;
    const uint32_t length;
    pd_t id{'\0'};
    pthread_mutex_t prm_access_mutex{};
    void *prmPtr{NULL};
} MParameterHandle_t;

class MParameterBase_ {
   public:
    inline const char *getName() {
        return ph.name;
    }

    inline const pd_t getId() {
        return ph.id;
    }

    inline const uint32_t getLength() {
        return ph.length;
    }

    /* Requires C++17 */
    inline static std::vector<MParameterHandle_t *> ptrMPH;
    inline static bool time_spec_offset{false};
    inline static uint32_t id_counter{0};

   protected:
    MParameterBase_(MParameterHandle_t &ph_, uint32_t prmSize_) : ph(ph_), prmSize(prmSize_) {
        if (ph.id == '\0') {
            pthread_mutex_lock(&gparameter_initialization_mutex);
            /* Check again in case it was initialized while waiting for mutex lock */
            if (ph.id == '\0') {
                ph.id = id_counter + 1;
                id_counter++;
                pthread_mutex_init(&ph.prm_access_mutex, NULL);
                ph.prmPtr = create_shared_memspace(prmSize);
                ptrMPH.push_back(&ph);
            }
            pthread_mutex_unlock(&gparameter_initialization_mutex);
        }
    }
    ~MParameterBase_() {
        //TODO: pop out of vector ptrMTH
        destroy_shared_memspace();
    }

    MParameterHandle_t &ph;
    uint32_t prmSize;

   private:
    inline void *create_shared_memspace(uint32_t size) {
        void *ptr = malloc(size);  //TODO: replace with custom per platform allocate
        memset(ptr, 0, size);
        return ptr;
    }

    inline void destroy_shared_memspace() {
        free(ph.prmPtr);  //TODO: replace with custom per platform dealloc
    }
}; 

template <typename MParameterT>
class MParameterPublisher : public MParameterBase_ {
    public:
    MParameterPublisher(MParameterT &ph) : MParameterBase_(ph, sizeof(MParameterT)){
        //TODO: publish or send to target?
    }

    inline uint32_t set(MParameterT &msg){
        //TODO: 
    }

    inline uint32_t get(){

    }
};

} // namespace MachineRPX


#endif // _PARAMETER_HPP_