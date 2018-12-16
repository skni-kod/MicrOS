#ifndef HEAP_H
#define HEAP_H

#define ENTRY_HEADER_SIZE 13

#include <stdint.h>
#include "heap_entry.h"
#include "../Virtual/virtual_memory_manager.h"

void *heap_kernel_alloc(uint32_t size, uint32_t align);
void *heap_user_alloc(uint32_t size, uint32_t align);
void *heap_alloc(uint32_t size, uint32_t align, bool supervisor);

void heap_kernel_dealloc(void *ptr);
void heap_user_dealloc(void *ptr);
void heap_dealloc(void *ptr, bool supervisor);

void heap_set_kernel_heap(void *heap_address);
void heap_set_user_heap(void *heap_address);

void heap_init_kernel_heap();
void heap_init_user_heap();
void heap_init_heap(bool supervisor);

void heap_kernel_dump();
void heap_user_dump();
void heap_dump(bool supervisor);

#endif