#ifndef PAGING_H
#define PAGING_H

#define PAGE_DIRECTORY_ADDRESS 0xC0001000
#define PAGE_TABLES_ADDRESS 0xC1100000

#include <stdint.h>
#include "paging_table_entry.h"

void paging_init();
void paging_remove_identity();
void paging_add_stack_guard();

uint32_t paging_map_page(uint32_t physical_address, uint32_t virtual_address);

#endif