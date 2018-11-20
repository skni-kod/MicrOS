#ifndef PAGING_H
#define PAGING_H

#define PAGE_DIRECTORY_ADDRESS 0xC0001000
#define PAGE_TABLES_ADDRESS 0xC1100000

#include "paging_table_entry.h"

void paging_init();
void paging_remove_identity();
void paging_add_stack_guard();

#endif