#include "paging.h"

paging_table_entry* page_directory = (paging_table_entry*)PAGE_DIRECTORY_ADDRESS;
paging_table_entry* page_tables = (paging_table_entry*)PAGE_TABLES_ADDRESS;

void paging_init()
{
    paging_remove_identity();
    paging_add_stack_guard();
}

void paging_remove_identity()
{
    // Remove 24 megabytes of identity mapping
    for(int i=0; i<6; i++)
    {
        page_directory[i].present = 0;
    }
}

void paging_add_stack_guard()
{
    paging_table_entry page_definition_with_stack = page_directory[768 + 4];
    uint32_t* page_address = (page_definition_with_stack.physical_page_address << 12) + 0xC0000000;

    paging_table_entry* page_with_stack = page_address;
    page_with_stack->present = 0;
}