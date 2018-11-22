#ifndef VIRTUAL_MEMORY_MANAGER_H
#define VIRTUAL_MEMORY_MANAGER_H

#include <stdint.h>
#include <stdbool.h>
#include "../../Paging/paging.h"
#include "../Physic/physical_memory_manager.h"

uint32_t virtual_memory_alloc_page();
bool virtual_memory_dealloc_page(uint32_t index);
void virtual_memory_set_base_page_index(uint32_t index);

#endif