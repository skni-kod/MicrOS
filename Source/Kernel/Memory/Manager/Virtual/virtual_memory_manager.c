#include "virtual_memory_manager.h"

uint32_t base_page_index;

uint32_t virtual_memory_alloc_page()
{
    uint32_t physical_address = physical_memory_alloc_page();
    int32_t first_available_page_index = paging_get_first_free_page_index(base_page_index);

    uint32_t virtual_address = first_available_page_index * 1024 * 1024 * 4;
    paging_map_page(physical_address, virtual_address);

    return virtual_address;
}

bool virtual_memory_dealloc_page(uint32_t index)
{
    uint32_t virtual_address = index * 1024 * 1024 * 4;
    paging_unmap_page(virtual_address);
}

void virtual_memory_set_base_page_index(uint32_t index)
{
    base_page_index = index;
}