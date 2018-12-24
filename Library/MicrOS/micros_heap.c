#include "micros_heap.h"

void *micros_heap_alloc(uint32_t size, uint32_t align)
{
    return (void *)micros_interrupt_2a(0x00, size, 0);
}

void *micros_heap_realloc(void *ptr, uint32_t size)
{
    return (void *)micros_interrupt_3a(0x01, ptr, size, 0);
}

void micros_heap_dealloc(void *ptr)
{
    return (void *)micros_interrupt_1a(0x02, ptr);
}

bool micros_verify_heap()
{
    return (bool *)micros_interrupt_0a(0x03);
}