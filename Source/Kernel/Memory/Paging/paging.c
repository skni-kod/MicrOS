#include "paging.h"

paging_table_entry* directory_table = DIRECTORY_TABLE_ADDRESS;

void paging_init()
{
    paging_remove_identity();
}

void paging_remove_identity()
{
    directory_table[0].present = 0;
}