#ifndef VIRTUAL_MEMORY_MANAGER_H
#define VIRTUAL_MEMORY_MANAGER_H

#include <stdint.h>
#include <stdbool.h>
#include "../../Paging/paging.h"

int32_t virtual_memory_alloc_page();
bool virtual_memory_dealloc_page(uint32_t index);

#endif