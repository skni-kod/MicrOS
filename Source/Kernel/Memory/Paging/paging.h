#ifndef PAGING_H
#define PAGING_H

#define DIRECTORY_TABLE_ADDRESS 0x1000
#define IDENTITY_PAGE_TABLE 0x2000
#define KERNEL_PAGE_TABLE 0x3000

#include "paging_table_entry.h"

void paging_init();
void paging_remove_identity();

#endif