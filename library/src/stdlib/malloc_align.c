#include "../stdlib.h"

void *malloc_align(size_t size, uint32_t align)
{
    return micros_heap_alloc(size, align);
}