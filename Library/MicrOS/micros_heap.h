#ifndef MICROS_HEAP_H
#define MICROS_HEAP_H

#include <stdbool.h>
#include "micros_interrupts.h"

void *micros_heap_alloc(uint32_t size, uint32_t align);
void *micros_heap_realloc(void *ptr, uint32_t size);
void micros_heap_dealloc(void *ptr);
uint32_t micros_heap_get_object_size(void *ptr);
bool micros_verify_integrity();

#endif