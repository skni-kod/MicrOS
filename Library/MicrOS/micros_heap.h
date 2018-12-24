#ifndef MICROS_HEAP_H
#define MICROS_HEAP_H

#include "micros_interrupts.h"

void *micros_heap_alloc(size_t size, uint32_t align);
void *micros_heap_realloc(void *ptr, size_t size);
void micros_heap_dealloc(void *ptr);

#endif