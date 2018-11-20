#ifndef PHYSICAL_MEMORY_ENTRY_H
#define PHYSICAL_MEMORY_ENTRY_H

typedef struct physical_memory_entry
{
    uint8_t type : 3;
} __attribute__((packed)) physical_memory_entry;

#endif