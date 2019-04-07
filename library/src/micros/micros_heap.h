#ifndef MICROS_HEAP_H
#define MICROS_HEAP_H

#include <stdbool.h>
#include "micros_interrupts.h"

typedef struct micros_heap_entry
{
    uint32_t size;
    uint8_t free;

    struct micros_heap_entry *next;
    struct micros_heap_entry *prev;
} __attribute__((packed)) micros_heap_entry;

void *micros_heap_alloc(uint32_t size, uint32_t align);
void *micros_heap_realloc(void *ptr, uint32_t size);
void micros_heap_dealloc(void *ptr);
uint32_t micros_heap_get_object_size(void *ptr);
bool micros_heap_verify_integrity();
micros_heap_entry *micros_heap_get_process_heap();

#endif