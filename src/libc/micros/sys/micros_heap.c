#ifndef __libk
#include "micros_heap.h"
#else
#include <memory/heap/heap.h>
#endif

void *micros_heap_alloc(uint32_t size, uint32_t align)
{
#ifndef __libk

    return (void *)micros_interrupt_2a(0x0000, size, align);

#else
    heap_kernel_alloc(size, align);
#endif
}

void *micros_heap_realloc(void *ptr, uint32_t size)
{
#ifndef __libk

    return (void *)micros_interrupt_3a(0x0001, (uint32_t)ptr, size, 0);

#else

#endif
}

void micros_heap_dealloc(void *ptr)
{
#ifndef __libk

    micros_interrupt_1a(0x0002, (uint32_t)ptr);

#else

#endif
}

uint32_t micros_heap_get_object_size(void *ptr)
{
#ifndef __libk

    return micros_interrupt_1a(0x0003, (uint32_t)ptr);

#else

#endif
}

bool micros_heap_verify_integrity()
{
#ifndef __libk

    return micros_interrupt_0a(0x0004);

#else

#endif
}

micros_heap_entry *micros_heap_get_process_heap()
{
#ifndef __libk

    return (micros_heap_entry *)micros_interrupt_0a(0x0005);

#else

#endif
}

void *micros_heap_kernel_alloc(uint32_t size, uint32_t align)
{
#ifndef __libk

    return (void *)micros_interrupt_2a(0x00FF, size, align);

#else

#endif
}

void *micros_heap_kernel_dealloc(void *address)
{
#ifndef __libk

    return (void *)micros_interrupt_1a(0x00FE, (uint32_t)address);

#else

#endif
}