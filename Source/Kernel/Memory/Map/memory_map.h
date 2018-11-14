#ifndef MEMORY_MAP_H
#define MEMORY_MAP_H

#define MEMORY_MAP_ADDRESS 0xc0000500

#include "../../Drivers/VGA/vga.h"
#include "../../../../Library/stdlib.h"
#include "memory_map_entry.h"

void memory_map_dump();

#endif