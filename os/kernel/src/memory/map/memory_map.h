#ifndef MEMORY_MAP_H
#define MEMORY_MAP_H

#define MEMORY_MAP_ENTRIES_COUNT 0xc0005C00
#define MEMORY_MAP_BUFFER MEMORY_MAP_ENTRIES_COUNT + 24

#include <stdint.h>
#include "drivers/vga/vga.h"
#include <stdlib.h>
#include "memory_map_entry.h"

memory_map_entry *memory_map_get();
uint8_t memory_map_get_entries_count();
void memory_map_dump();

#endif