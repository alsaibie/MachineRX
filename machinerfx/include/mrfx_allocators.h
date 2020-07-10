#ifndef _MRFX_ALLOCATORS_H_
#define _MRFX_ALLOCATORS_H_

#include <stdint.h>

typedef struct mrfx_allocator_t {
    void * (*malloc)(size_t size);
    void * (*free)(void *ptr);
}mrfx_allocator_t;

extern mrfx_allocator_t mrfx_allocator;


#endif // _MRFX_ALLOCATORS_H_