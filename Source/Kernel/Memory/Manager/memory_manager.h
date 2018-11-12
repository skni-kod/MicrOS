#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include <stdint.h>
#include "object_header.h"

void memory_manager_init_memory(uint32_t initial_size);
void memory_manager_set_base_address(uint32_t address);
void memory_manager_alloc();
void memory_manager_free();

#endif