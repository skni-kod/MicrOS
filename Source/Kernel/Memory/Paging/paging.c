#include "paging.h"

paging_table_entry* page_directory = (paging_table_entry*)PAGE_DIRECTORY_ADDRESS;
paging_table_entry* page_tables = (paging_table_entry*)PAGE_TABLES_ADDRESS;

void paging_init()
{
    paging_remove_identity();
}

void paging_remove_identity()
{
    page_directory[0].present = 0;
}