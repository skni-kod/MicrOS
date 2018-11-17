#ifndef MEMORY_MAP_H
#define MEMORY_MAP_H

#define MEMORY_MAP_ENTRIES_COUNT 0xc0000500
#define MEMORY_MAP_BUFFER MEMORY_MAP_ENTRIES_COUNT + 24

#include <stdint.h>
#include "../../Drivers/VGA/vga.h"
#include "../../../../Library/stdlib.h"
#include "memory_map_entry.h"

void memory_map_dump();

#endif