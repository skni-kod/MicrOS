#ifndef HEAP_ENTRY_H
#define HEAP_ENTRY_H

#include <stdint.h>

typedef struct heap_entry
{
    uint32_t size;
    uint8_t free;

    struct heap_entry *next;
    struct heap_entry *prev;
} __attribute__((packed)) heap_entry;

#endif