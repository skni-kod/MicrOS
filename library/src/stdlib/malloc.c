#include "../stdlib.h"

void *malloc(size_t size)
{
    return micros_heap_alloc(size, 0);
}