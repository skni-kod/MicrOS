#ifndef PHYSICAL_MEMORY_MANAGER_H
#define PHYSICAL_MEMORY_MANAGER_H

#include <stdbool.h>
#include "../../Map/memory_map.h"
#include "../../Map/memory_map_entry.h"
#include "../../../Drivers/VGA/vga.h"
#include "../../../Drivers/Keyboard/keyboard.h"
#include <stdlib.h>
#include "physical_memory_entry.h"
#include "physical_memory_entry_type.h"
#include "physical_memory_stats.h"

void physical_memory_init();
uint32_t physical_memory_alloc_page();
bool physical_memory_dealloc_page(uint32_t index);
void physical_memory_dump();
void draw_4MB_array(uint64_t number_of_sector);
void memoryViewer();

void physical_memory_get_stats(physical_memory_stats *stats);

#endif