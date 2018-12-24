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

// TODO: This function should return 0 if there is no available memory to allocate instead of panic screen.
void *heap_alloc(uint32_t size, uint32_t align, bool supervisor)
{
    heap_entry *current_entry = supervisor ? kernel_heap : user_heap;

    while (true)
    {
        uint32_t align_fix = align == 0 ? 0 : align - (((uint32_t)current_entry) % align) - ENTRY_HEADER_SIZE;
        uint32_t extra_headers_size = align == 0 ? 0 : ENTRY_HEADER_SIZE;

        if (current_entry->free)
        {
            uint32_t required_size = size + align_fix + extra_headers_size;
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
                    current_entry->next = (heap_entry *)((uint32_t)current_entry + align_fix);
                    current_entry->next->prev = current_entry;
                    current_entry->size = align_fix - ENTRY_HEADER_SIZE;
                    current_entry->free = 1;

                    current_entry = current_entry->next;
                }

                current_entry->next = (heap_entry *)((uint32_t)current_entry + size + ENTRY_HEADER_SIZE);

                if (current_entry->size != required_size)
                {
                    current_entry->next->next = next_entry;
                    current_entry->next->prev = current_entry;
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
                        virtual_memory_alloc_page(supervisor);
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

void *heap_realloc(void *ptr, uint32_t size, uint32_t align, bool supervisor)
{
    heap_entry *old_entry = heap_dealloc(ptr, supervisor);
    void *new_ptr = heap_alloc(size, align, supervisor);

    memcpy(new_ptr, ptr, old_entry->size);
    return new_ptr;
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

    heap->size = 4 * 1024 * 1024;
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

bool heap_kernel_verify_heap()
{
    return heap_verify_heap(true);
}

bool heap_user_verify_heap()
{
    return heap_verify_heap(false);
}

bool heap_verify_heap(bool supervisor)
{
    heap_entry *current_entry = supervisor ? kernel_heap : user_heap;

    uint32_t total_size = 0;
    uint32_t allocated_pages = virtual_memory_get_allocated_pages_count(supervisor);

    while (true)
    {
        total_size += current_entry->size + ENTRY_HEADER_SIZE;

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

    return (total_size / 1024 / 1024 / 4) + 1 == allocated_pages;
}