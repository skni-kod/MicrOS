#ifndef PHYSICAL_MEMORY_MANAGER_H
#define PHYSICAL_MEMORY_MANAGER_H

#include "../../Map/memory_map.h"
#include "../../Map/memory_map_entry.h"
#include "../../../Drivers/VGA/vga.h"
#include "physical_memory_entry.h"
#include "physical_memory_entry_type.h"

void physical_memory_init();
void physical_memory_dump();

#endif