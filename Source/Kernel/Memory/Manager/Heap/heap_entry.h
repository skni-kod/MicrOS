#ifndef HEAP_ENTRY_H
#define HEAP_ENTRY_H
#pragma pack(1)

#include <stdint.h>

typedef struct heap_entry
{
    uint32_t size : 31;
    uint8_t free : 1;

    struct heap_entry* next;
    struct heap_entry* prev;
} heap_entry;

#endif