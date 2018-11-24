#include "virtual_memory_manager.h"

uint32_t base_page_index;

uint32_t virtual_memory_alloc_page()
{
    uint32_t physical_page_index = physical_memory_alloc_page();
    uint32_t virtual_page_index = paging_get_first_free_page_index(base_page_index);

    paging_map_page(physical_page_index, virtual_page_index);

    return virtual_page_index;
}

bool virtual_memory_dealloc_page(uint32_t page_index)
{
    uint32_t physical_index = paging_get_physical_index_of_virtual_page(page_index) / 1024 / 4;

    paging_unmap_page(page_index);
    physical_memory_dealloc_page(physical_index);
}

uint32_t virtual_memory_get_allocated_pages_count()
{
    uint32_t count = 0;
    for(int i = base_page_index; i < 1024; i++)
    {
        if(paging_is_page_mapped(i))
        {
            count++;
        }
        else break;
    }

    return count;
}

void virtual_memory_set_base_page_index(uint32_t index)
{
    base_page_index = index;
}