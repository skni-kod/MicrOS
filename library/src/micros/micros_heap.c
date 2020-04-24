#include "micros_heap.h"

void *micros_heap_alloc(uint32_t size, uint32_t align)
{
    return (void *)micros_interrupt_2a(0x00, size, align);
}

void *micros_heap_realloc(void *ptr, uint32_t size)
{
    return (void *)micros_interrupt_3a(0x01, (uint32_t)ptr, size, 0);
}

void micros_heap_dealloc(void *ptr)
{
    micros_interrupt_1a(0x02, (uint32_t)ptr);
}

uint32_t micros_heap_get_object_size(void *ptr)
{
    return micros_interrupt_1a(0x03, (uint32_t)ptr);
}

bool micros_heap_verify_integrity()
{
    return micros_interrupt_0a(0x04);
}

micros_heap_entry *micros_heap_get_process_heap()
{
    return (micros_heap_entry *)micros_interrupt_0a(0x05);
}