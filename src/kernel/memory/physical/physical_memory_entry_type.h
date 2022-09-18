#ifndef PHYSICAL_MEMORY_ENTRY_TYPE_H
#define PHYSICAL_MEMORY_ENTRY_TYPE_H

typedef enum physical_memory_entry_type
{
    physical_memory_free = 0,
    physical_memory_reserved = 1,
    physical_memory_filled = 2,
    physical_memory_not_available = 3
} physical_memory_entry_type;

#endif