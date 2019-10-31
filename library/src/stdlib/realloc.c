#include "../stdlib.h"

void *realloc(void *ptr, size_t size)
{
    return micros_heap_realloc(ptr, size);
}