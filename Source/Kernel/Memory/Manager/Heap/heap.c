#include "heap.h"

heap_entry* heap;

uint32_t* heap_alloc(uint32_t size)
{

}

void heap_dealloc(uint32_t* ptr)
{

}

void heap_set_base_page_index(uint32_t index)
{
    heap = index * 1024 * 1024 * 4;
}

void heap_clear()
{
    uint32_t allocated_virtual_pages = virtual_memory_get_allocated_pages_count();
    if(allocated_virtual_pages == 0)
    {
        heap = virtual_memory_alloc_page() * 1024 * 1024 * 4;
    }

    heap->size = 0xFFFFFFFF - (uint32_t)heap;
    heap->free = 1;
    heap->next = 0;
}