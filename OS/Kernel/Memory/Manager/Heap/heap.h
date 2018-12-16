#ifndef HEAP_H
#define HEAP_H

#define ENTRY_HEADER_SIZE 13

#include <stdint.h>
#include "heap_entry.h"
#include "../Virtual/virtual_memory_manager.h"

void *heap_alloc(uint32_t size, uint32_t align);
void heap_dealloc(void *ptr);
void heap_set_root_address(void *heap_root_address);
void heap_init_root();
void heap_dump();

#endif