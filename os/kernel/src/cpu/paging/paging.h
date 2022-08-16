#ifndef PAGING_H
#define PAGING_H

#define KERNEL_PAGE_DIRECTORY_ADDRESS 0xC0006000
#define PAGE_TABLES_ADDRESS 0xC1100000

#include <stdint.h>
#include <stdbool.h>
#include "paging_table_entry.h"
#include "cpu/panic/panic_screen.h"

void paging_init();
void paging_remove_identity();
void paging_add_stack_guard();
paging_table_entry *paging_get_page_directory();
paging_table_entry *paging_get_kernel_page_directory();
void paging_set_page_directory(void *address);

void paging_map_page(uint32_t physical_page_index, uint32_t virtual_page_index, bool supervisor);
void paging_unmap_page(uint32_t page_index);
uint32_t paging_get_first_free_page_index(uint32_t from_index);
uint32_t paging_get_physical_index_of_virtual_page(uint32_t virtual_index);
bool paging_is_page_mapped(uint32_t page_index);

#endif