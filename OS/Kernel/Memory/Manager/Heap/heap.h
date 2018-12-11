#ifndef HEAP_H
#define HEAP_H

#define ENTRY_HEADER_SIZE 13

#include <stdint.h>
#include "heap_entry.h"
#include "../Virtual/virtual_memory_manager.h"

void* heap_alloc(uint32_t size);
void heap_dealloc(void* ptr);
void heap_set_base_page_index(uint32_t index);
void heap_clear();
void heap_dump();

#endif