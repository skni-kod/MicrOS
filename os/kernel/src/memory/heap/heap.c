#include "heap.h"

heap_entry *kernel_heap;
heap_entry *user_heap;

void *heap_kernel_alloc(uint32_t size, uint32_t align)
{
    return heap_alloc(size, align, true);
}

void *heap_user_alloc(uint32_t size, uint32_t align)
{
    return heap_alloc(size, align, false);
}

void *heap_alloc(uint32_t size, uint32_t align, bool supervisor)
{
    heap_entry *current_entry = supervisor ? kernel_heap : user_heap;

    while (true)
    {
        uint32_t align_fix = 0;

        if (align != 0)
        {
            align_fix = align - (((uint32_t)current_entry) % align);
            if (align_fix < ENTRY_HEADER_SIZE * 2)
            {
                align_fix += align;
            }
        }

        if (current_entry->free)
        {
            uint32_t required_size = size + align_fix;
            if (current_entry->size > required_size)
            {
                required_size += ENTRY_HEADER_SIZE;
            }

            if (current_entry->size >= required_size)
            {
                heap_entry *next_entry = current_entry->next;

                uint32_t updated_free_size = current_entry->size - required_size;
                if (align_fix != 0)
                {
                    current_entry->next = (heap_entry *)((uint32_t)current_entry + align_fix - ENTRY_HEADER_SIZE);
                    current_entry->next->prev = current_entry;
                    current_entry->size = align_fix - 2 * ENTRY_HEADER_SIZE;
                    current_entry->free = 1;

                    current_entry = current_entry->next;
                    updated_free_size += ENTRY_HEADER_SIZE;
                }

                current_entry->next = (heap_entry *)((uint32_t)current_entry + size + ENTRY_HEADER_SIZE);

                if (current_entry->next != next_entry)
                {
                    current_entry->next->next = next_entry;
                    current_entry->next->prev = current_entry;
                    
                    if(current_entry->next->next != 0)
                    {
                        current_entry->next->next->prev = current_entry->next;
                    }
                    
                    current_entry->next->size = updated_free_size;
                    current_entry->next->free = 1;
                }

                current_entry->size = size;
                current_entry->free = 0;
                
                return (void *)((uint32_t)current_entry + ENTRY_HEADER_SIZE);
            }
            else
            {
                if (current_entry->next == 0)
                {
                    while (current_entry->size < size + align_fix + ENTRY_HEADER_SIZE)
                    {
                        if(virtual_memory_alloc_page(supervisor) == 0)
                        {
                            return 0;
                        }
                        
                        current_entry->size += 4 * 1024 * 1024;
                    }

                    continue;
                }
            }
        }

        current_entry = current_entry->next;
    }
}

heap_entry *heap_kernel_dealloc(void *ptr)
{
    return heap_dealloc(ptr, true);
}

heap_entry *heap_user_dealloc(void *ptr)
{
    return heap_dealloc(ptr, false);
}

heap_entry *heap_dealloc(void *ptr, bool supervisor)
{
    heap_entry *entry = (heap_entry *)((uint32_t)ptr - ENTRY_HEADER_SIZE);
    entry->free = 1;

    if (entry->prev != 0 && entry->prev->free)
    {
        entry->prev->size += entry->size + ENTRY_HEADER_SIZE;
        entry->prev->next = entry->next;

        if (entry->next != 0)
        {
            entry->next->prev = entry->prev;
        }

        entry = entry->prev;
    }

    if (entry->next != 0)
    {
        if (entry->next->free)
        {
            void *next_entry = (void *)((uint32_t)entry->next + ENTRY_HEADER_SIZE);
            heap_dealloc(next_entry, supervisor);
        }
    }
    else
    {
        while ((float)entry->size / 1024 / 1024 > 4)
        {
            virtual_memory_dealloc_last_page(supervisor);
            entry->size -= 4 * 1024 * 1024;
        }
    }
    
    return entry;
}

void *heap_kernel_realloc(void *ptr, uint32_t size, uint32_t align)
{
    return heap_realloc(ptr, size, align, true);
}

void *heap_user_realloc(void *ptr, uint32_t size, uint32_t align)
{
    return heap_realloc(ptr, size, align, false);
}

uint32_t heap_get_object_size(void *ptr)
{
    heap_entry *entry = (heap_entry *)((uint32_t)ptr - ENTRY_HEADER_SIZE);
    return entry->size;
}

void *heap_realloc(void *ptr, uint32_t size, uint32_t align, bool supervisor)
{
    if(ptr == NULL) return heap_alloc(size, align, supervisor);
    
    uint32_t old_entry_size = ((heap_entry *)((uint32_t)ptr - ENTRY_HEADER_SIZE))->size;
    void *new_ptr = heap_alloc(size, align, supervisor);

    memmove(new_ptr, ptr, size < old_entry_size ? size : old_entry_size);
    heap_dealloc(ptr, supervisor);
    
    return new_ptr;
}

heap_entry *heap_get_kernel_heap()
{
    return kernel_heap;
}

heap_entry *heap_get_user_heap()
{
    return user_heap;
}

void heap_set_kernel_heap(void *heap_address)
{
    kernel_heap = heap_address;
    virtual_memory_set_kernel_base_page_index((uint32_t)kernel_heap / 1024 / 1024 / 4);
}

void heap_set_user_heap(void *heap_address)
{
    user_heap = heap_address;
    virtual_memory_set_user_base_page_index((uint32_t)user_heap / 1024 / 1024 / 4);
}

void heap_init_kernel_heap()
{
    heap_init_heap(true);
}

void heap_init_user_heap()
{
    heap_init_heap(false);
}

void heap_init_heap(bool supervisor)
{
    heap_entry *heap = supervisor ? kernel_heap : user_heap;
    uint32_t allocated_virtual_pages = virtual_memory_get_allocated_pages_count(supervisor);

    if (allocated_virtual_pages == 0)
    {
        heap = (heap_entry *)(virtual_memory_alloc_page(supervisor) * 1024 * 1024 * 4);
    }

    heap->size = (4 * 1024 * 1024) - ((uint32_t)heap % (4 * 1024 * 1024)) - ENTRY_HEADER_SIZE;
    heap->free = 1;
    heap->next = 0;
    heap->prev = 0;
}

void heap_kernel_dump()
{
    heap_dump(true);
}

void heap_user_dump()
{
    heap_dump(false);
}

void heap_dump(bool supervisor)
{
    heap_entry *current_entry = supervisor ? kernel_heap : user_heap;

    uint32_t index = 0;
    char buffer[16];

    while (current_entry != 0)
    {
        vga_printstring("Entry id=");
        itoa(index, buffer, 10);
        vga_printstring(buffer);
        vga_printstring(", free=");
        itoa(current_entry->free, buffer, 10);
        vga_printstring(buffer);
        vga_printstring(", size=");
        itoa(current_entry->size, buffer, 10);
        vga_printstring(buffer);
        vga_printstring("\n");

        current_entry = current_entry->next;
        index++;
    }
}

bool heap_kernel_verify_integrity()
{
    return heap_verify_integrity(true);
}

bool heap_user_verify_integrity()
{
    return heap_verify_integrity(false);
}

bool heap_verify_integrity(bool supervisor)
{
    heap_entry *current_entry = supervisor ? kernel_heap : user_heap;

    uint32_t total_size = 0;
    uint32_t allocated_pages = virtual_memory_get_allocated_pages_count(supervisor);

    while (true)
    {
        total_size += current_entry->size + ENTRY_HEADER_SIZE;
        
        if(current_entry->prev != 0 && current_entry->prev->next != current_entry)
        {
            return false;
        }
        
        if(current_entry->next != 0 && current_entry->next->prev != current_entry)
        {
            return false;
        }
        
        if (current_entry->next != 0 && (uint32_t)current_entry->next - (uint32_t)current_entry != current_entry->size + ENTRY_HEADER_SIZE)
        {
            return false;
        }

        if (current_entry->next == 0)
        {
            break;
        }

        current_entry = current_entry->next;
    }

    if (supervisor)
    {
        return total_size == allocated_pages * 1024 * 1024 * 4;
    }
    else
    {
        return true;
    }
}