#include "../stdlib.h"

void *calloc(size_t num, size_t size)
{
    size_t total_size = num * size;

    void *ptr = micros_heap_alloc(total_size, 0);
    memset(ptr, 0, total_size);

    return ptr;
}