#ifndef MEMORY_MAP_ENTRY_H
#define MEMORY_MAP_ENTRY_H

#include <stdint.h>

typedef enum memory_map_entry_type
{
    free = 1,
    reserved = 2,
    acpi_relaimable_memory = 3,
    acpi_nvs_memory = 4,
    bad_memory = 5
} memory_map_entry_type;

typedef struct memory_map_entry
{
    uint64_t base_address;
    uint64_t length;
    memory_map_entry_type type;
    uint32_t acpi;
} memory_map_entry;

#endif