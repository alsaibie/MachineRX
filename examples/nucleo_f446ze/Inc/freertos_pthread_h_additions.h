#ifndef _FREERTOS_PTHREAD_C_ADDITIONS_H_
#define _FREERTOS_PTHREAD_C_ADDITIONS_H_

int pthread_setname_np(pthread_t thread, const char *name);
int pthread_getname_np(pthread_t thread, char *buf, size_t len);

#endif