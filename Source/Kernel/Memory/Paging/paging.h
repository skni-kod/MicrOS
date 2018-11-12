#ifndef PAGING_H
#define PAGING_H

#define PAGE_DIRECTORY_ADDRESS 0x200000
#define PAGE_TABLES_ADDRESS 0x201000

#include "paging_table_entry.h"

void paging_init();
void paging_remove_identity();

#endif