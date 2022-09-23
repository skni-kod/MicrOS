#include <klibrary/kbuffer.h>

kbuffer_t *kbuffer_init(uint32_t block_size, uint32_t block_count)
{
    kbuffer_t *buffer;
    size_t block_size_in_memory = (sizeof(kentry_t) + block_size);
    size_t buffer_size = sizeof(kbuffer_t) + block_size_in_memory * block_count;

    buffer = heap_kernel_alloc(buffer_size, 0);

    if (!buffer)
        return -1;

    buffer->block_size = block_size;
    buffer->block_count = block_count;
    // offset of buffer->data, points last used part of memory
    buffer->ptr = 0;

    kentry_t *entry;
    for (uint32_t entry_ptr = 0; entry_ptr < buffer->block_count; entry_ptr++)
    {
        entry = (kentry_t*)((uint32_t)buffer->entries + (entry_ptr * (uint32_t)block_size_in_memory));
        entry->size = entry_ptr + 1;
        entry->used = 0;
    }

    return buffer;
}

void *kbuffer_get(kbuffer_t *buffer, uint32_t size)
{
    // ptr points to last used block of data, so we know is its size
    uint32_t start_point = buffer->ptr;
    size_t block_size_in_memory = (sizeof(kentry_t) + buffer->block_size);
    uint32_t req_blocks = buffer->block_size / size;

    if (buffer->block_size % size)
        req_blocks++;

    kentry_t *entry;

    while (1)
    {
        // when we can not fit continous block of memory at the end backing memory
        // go back to begin, and start searching for fitting place:
        if (buffer->ptr + req_blocks > buffer->block_count)
            buffer->ptr = 0;

        entry = (uint32_t)buffer->entries + (buffer->ptr * block_size_in_memory);
        // go right after the entry and look for place:
        if (entry->used)
        {
            buffer->ptr += entry->size;
            if (buffer->ptr >= buffer->block_count)
                buffer->ptr = 0;
            continue;
        }
        else
        {
            entry->used = 1;
            entry->size = req_blocks;
            buffer->ptr += req_blocks;
            return entry->data;
        }

        // unable to find place
        if (start_point == buffer->ptr)
            return NULL;
    }
}

void kbuffer_drop(void *ptr)
{
    // mark entry as free
    *((uint8_t *)ptr - 4) = 0;
}
