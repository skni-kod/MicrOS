#include "micros_heap.h"

void *micros_heap_alloc(uint32_t size, uint32_t align)
{
    return (void *)micros_interrupt_2a(0x0000, size, align);
}

void *micros_heap_realloc(void *ptr, uint32_t size)
{
    return (void *)micros_interrupt_3a(0x0001, (uint32_t)ptr, size, 0);
}

void micros_heap_dealloc(void *ptr)
{
    micros_interrupt_1a(0x0002, (uint32_t)ptr);
}

uint32_t micros_heap_get_object_size(void *ptr)
{
    return micros_interrupt_1a(0x0003, (uint32_t)ptr);
}

bool micros_heap_verify_integrity()
{
    return micros_interrupt_0a(0x0004);
}

micros_heap_entry *micros_heap_get_process_heap()
{
    return (micros_heap_entry *)micros_interrupt_0a(0x0005);
}