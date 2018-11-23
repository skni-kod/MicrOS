#include "heap.h"

heap_entry* heap;

uint32_t* heap_alloc(uint32_t size)
{
    heap_entry* current_entry = heap;
    
    while(true)
    {
        if(current_entry->free && current_entry->size >= size)
        {
            if(current_entry->next == 0)
            {
                uint32_t updated_free_size = current_entry->size - size - 8;
                current_entry->next = (uint32_t)current_entry + size + 8;
                current_entry->size = size;
                current_entry->free = 0;

                current_entry->next->next = 0;
                current_entry->next->prev = current_entry;
                current_entry->next->size = updated_free_size;
                current_entry->next->free = 1;

                return (uint32_t)current_entry + 8;
            }
            else
            {

            }
        }

        current_entry = current_entry->next;
    }
}

void heap_dealloc(uint32_t* ptr)
{
    heap_entry* entry = (uint32_t)ptr - 8;
    entry->free = 1;

    if(entry->prev != 0 && entry->prev->free)
    {
        entry->prev->size += entry->size + 8;
        entry->prev->next = entry->next;
        entry->next->prev = entry->prev;
    }

    if(entry->next != 0 && entry->next->free)
    {
        heap_dealloc((uint32_t)entry->next + 8);
    }
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
    heap->prev = 0;
}