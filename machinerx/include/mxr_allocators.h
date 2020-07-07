
#ifndef _MXR_ALLOCATORS_H_
#define _MXR_ALLOCATORS_H_

#include <stdint.h>

typedef struct mxr_allocator_t {
    void * (*malloc)(size_t size);
    void * (*free)(void *ptr);
}mxr_allocator_t;

extern mxr_allocator_t mxr_allocator;


#endif // _MXR_ALLOCATORS_H_