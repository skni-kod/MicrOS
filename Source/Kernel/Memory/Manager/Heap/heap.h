#ifndef HEAP_H
#define HEAP_H

#include <stdint.h>
#include "heap_entry.h"
#include "../Virtual/virtual_memory_manager.h"

uint32_t* heap_alloc(uint32_t size);
void heap_dealloc(uint32_t* ptr);
void heap_set_base_page_index(uint32_t index);
void heap_clear();

#endif