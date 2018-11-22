#include "virtual_memory_manager.h"

uint32_t base_page_index;

uint32_t virtual_memory_alloc_page()
{
    uint32_t physical_page_index = physical_memory_alloc_page();
    int32_t virtual_page_index = paging_get_first_free_page_index(base_page_index);

    paging_map_page(physical_page_index, virtual_page_index);

    return virtual_page_index;
}

bool virtual_memory_dealloc_page(uint32_t page_index)
{
    paging_unmap_page(page_index);
    physical_memory_dealloc_page(page_index);
}

void virtual_memory_set_base_page_index(uint32_t index)
{
    base_page_index = index;
}