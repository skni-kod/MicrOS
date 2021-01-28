#include "paging.h"

paging_table_entry *page_directory = (paging_table_entry *)KERNEL_PAGE_DIRECTORY_ADDRESS;
paging_table_entry *page_tables = (paging_table_entry *)PAGE_TABLES_ADDRESS;

void paging_init()
{
    paging_remove_identity();
    paging_add_stack_guard();
}

void paging_remove_identity()
{
    // Remove 24 megabytes of identity mapping
    for (int i = 0; i < 6; i++)
    {
        page_directory[i].present = 0;
    }
}

void paging_add_stack_guard()
{
    paging_table_entry *page_directory_entry_with_stack = &page_directory[768 + 4];
    uint32_t *page_address = (uint32_t *)((page_directory_entry_with_stack->physical_page_address << 12) + 0xC0000000);

    paging_table_entry *page_with_stack = (paging_table_entry *)page_address;
    page_with_stack->present = 0;
}

paging_table_entry *paging_get_page_directory()
{
    return page_directory;
}

paging_table_entry *paging_get_kernel_page_directory()
{
    return (paging_table_entry *)KERNEL_PAGE_DIRECTORY_ADDRESS;
}

void paging_set_page_directory(void *address)
{
    page_directory = (paging_table_entry *)address;
    __asm__("mov %0, %%eax\n"
            "mov %%eax, %%cr3"
            :
            : "g"((uint32_t)address - 0xC0000000)
            : "eax");
}

void paging_map_page(uint32_t physical_page_index, uint32_t virtual_page_index, bool supervisor)
{
    paging_table_entry *page_directory_entry = (paging_table_entry *)((uint32_t)page_directory + (virtual_page_index * 4));
    paging_table_entry *page_table = (paging_table_entry *)((uint32_t)page_tables + (physical_page_index << 12));

    for (int i = 0; i < 1024; i++)
    {
        page_table[i].physical_page_address = (physical_page_index * 1024) + i;
        page_table[i].present = 1;
        page_table[i].read_write = 1;
        page_table[i].user_supervisor = !supervisor;
    }

    page_directory_entry->physical_page_address = ((uint32_t)page_table - 0xC0000000) >> 12;
    page_directory_entry->present = 1;
    page_directory_entry->read_write = 1;
    page_directory_entry->user_supervisor = !supervisor;
}

void paging_unmap_page(uint32_t page_index)
{
    paging_table_entry *page_directory_entry = (paging_table_entry *)((uint32_t)page_directory + (page_index * 4));
    page_directory_entry->present = 0;
}

uint32_t paging_get_first_free_page_index(uint32_t from_index)
{
    for (int i = from_index; i < 1024; i++)
    {
        if (page_directory[i].present == 0)
        {
            return i;
        }
    }

    return 0;
}

uint32_t paging_get_physical_index_of_virtual_page(uint32_t page_index)
{
    paging_table_entry *page_directory_entry = (paging_table_entry *)((uint32_t)page_directory + (page_index * 4));
    paging_table_entry *page_table = (paging_table_entry *)(0xC0000000 + ((uint32_t)page_directory_entry->physical_page_address << 12));

    return page_table->physical_page_address / 1024;
}

bool paging_is_page_mapped(uint32_t page_index)
{
    paging_table_entry *page_directory_entry = (paging_table_entry *)((uint32_t)page_directory + (page_index * 4));
    return page_directory_entry->present;
}