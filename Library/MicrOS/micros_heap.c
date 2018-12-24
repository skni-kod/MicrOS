#include "micros_heap.h"

void *micros_heap_alloc(size_t size, uint32_t align)
{
    return (void *)call_interrupt_2a(0x00, size, 0);
}

void *micros_heap_realloc(void *ptr, size_t size)
{
    return (void *)call_interrupt_3a(0x01, ptr, size, 0);
}

void micros_heap_dealloc(void *ptr)
{
    return (void *)call_interrupt_1a(0x02, ptr);
}