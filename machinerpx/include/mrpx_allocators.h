
#ifndef _MRPX_ALLOCATORS_H_
#define _MRPX_ALLOCATORS_H_

#include <stdint.h>

typedef struct mrpx_allocator_t {
    void * (*malloc)(size_t size);
    void * (*free)(void *ptr);
}mrpx_allocator_t;

extern mrpx_allocator_t mrpx_allocator;


#endif // _MRPX_ALLOCATORS_H_