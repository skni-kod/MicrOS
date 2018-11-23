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

void paging_map_page(uint32_t physical_page_index, uint32_t virtual_page_index)
{
    paging_table_entry* page_directory = PAGE_DIRECTORY_ADDRESS + (virtual_page_index * 4);
    paging_table_entry* page_table = page_tables + (virtual_page_index * 1024);

    for(int i=0; i<1024; i++)
    {
        page_table[i].physical_page_address = (physical_page_index << 12) + i;
        page_table[i].present = 1;
    }

    page_directory->physical_page_address = ((uint32_t)page_table - 0xC0000000) >> 12;
    page_directory->present = 1;
    page_directory->read_write = 1;
}

void paging_unmap_page(uint32_t page_index)
{
    paging_table_entry* page_directory = PAGE_DIRECTORY_ADDRESS + (page_index * 4);
    page_directory->present = 0;
}

int32_t paging_get_first_free_page_index(uint32_t from_index)
{
    for(int i=from_index; i<1024; i++)
    {
        if(page_directory[i].present == 0)
        {
            return i;
        }
    }

    showPanicScreen(0xA0, "Out of memory");
}

uint32_t paging_get_physical_index_of_virtual_page(uint32_t page_index)
{
    paging_table_entry* page_directory = PAGE_DIRECTORY_ADDRESS + (page_index * 4);
    paging_table_entry* page_table = 0xC0000000 + ((uint32_t)page_directory->physical_page_address << 12);

    return page_table->physical_page_address;
}

bool paging_is_page_mapped(uint32_t page_index)
{
    paging_table_entry* page_directory = PAGE_DIRECTORY_ADDRESS + (page_index * 4);
    return page_directory->present;
}