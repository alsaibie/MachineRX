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
 * File: freertos_pthread_c_additions.h
 * Project: MachineRX
 * Author: Ali AlSaibie (ali.alsaibie@ku.edu.kw)
 * -----
 * Modified By: Ali AlSaibie (ali.alsaibie@ku.edu.kw>)
 */

#ifndef _FREERTOS_PTHREAD_C_ADDITIONS_H_
#define _FREERTOS_PTHREAD_C_ADDITIONS_H_

int pthread_setname_np(pthread_t thread, const char *name){
    int iStatus = 0;
    pthread_internal_t * pxThread = ( pthread_internal_t * ) thread;
    
    pcTaskSetName(pxThread->TaskHandle_t, name);

    return iStatus;
// pcTaskSetName

}

int pthread_getname_np(pthread_t thread, char *buf, size_t len){
    int iStatus = 0;
    pthread_internal_t * pxThread = ( pthread_internal_t * ) thread;
    
    char * buf_ = pcTaskGetName(pxThread->TaskHandle_t);
    memcpy(buf,buf_,len);

    if(buf[0] == '\0'){
        iStatus = 1; // TODO: correct return flag value
    }
    return iStatus;
}


#endif