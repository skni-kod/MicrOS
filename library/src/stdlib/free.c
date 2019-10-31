#include "../stdlib.h"

void free(void *ptr)
{
    micros_heap_dealloc(ptr);
}