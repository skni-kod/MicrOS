#include "virtual_memory_manager.h"

uint32_t kernel_base_page_index;
uint32_t user_base_page_index;

uint32_t virtual_memory_alloc_page(bool supervisor)
{
    uint32_t base_page_index = supervisor ? kernel_base_page_index : user_base_page_index;

    uint32_t physical_page_index = physical_memory_alloc_page();
    uint32_t virtual_page_index = paging_get_first_free_page_index(base_page_index);

    if(physical_page_index == 0 || virtual_page_index == 0)
    {
        return 0;
    }
    
    paging_map_page(physical_page_index, virtual_page_index, supervisor);
    return virtual_page_index;
}

bool virtual_memory_dealloc_page(uint32_t page_index)
{
    uint32_t physical_index = paging_get_physical_index_of_virtual_page(page_index);
    paging_unmap_page(page_index);

    return physical_memory_dealloc_page(physical_index);
}

bool virtual_memory_dealloc_last_page(bool supervisor)
{
    uint32_t base_page_index = supervisor ? kernel_base_page_index : user_base_page_index;
    for (int i = base_page_index; i < 1024; i++)
    {
        if (!paging_is_page_mapped(i + 1))
        {
            uint32_t physical_page_index = paging_get_physical_index_of_virtual_page(i);
            paging_unmap_page(i);
            
            return physical_memory_dealloc_page(physical_page_index);
        }
    }

    return false;
}

uint32_t virtual_memory_get_allocated_pages_count(bool supervisor)
{
    uint32_t count = 0;
    uint32_t base_page_index = supervisor ? kernel_base_page_index : user_base_page_index;

    for (int i = base_page_index; i < 1024; i++)
    {
        if (paging_is_page_mapped(i))
        {
            count++;
        }
        else
        {
            break;
        }
    }

    return count;
}

uint32_t virtual_memory_get_kernel_base_page_index()
{
    return kernel_base_page_index;
}

uint32_t virtual_memory_get_user_base_page_index()
{
    return user_base_page_index;
}

void virtual_memory_set_kernel_base_page_index(uint32_t index)
{
    kernel_base_page_index = index;
}

void virtual_memory_set_user_base_page_index(uint32_t index)
{
    user_base_page_index = index;
}