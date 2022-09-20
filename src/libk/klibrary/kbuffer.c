#include <klibrary/kbuffer.h>

int kbuffer_init(kbuffer_t *buffer, uint16_t data_size, uint16_t entry_count)
{
    size_t entry_size = (sizeof(kentry_t) + data_size);
    size_t buffer_size = sizeof(kbuffer_t) + entry_size * entry_count;
    buffer = heap_kernel_alloc(buffer_size, 0);
    if (!buffer)
        return -1;
    memset(buffer, 0, buffer_size);
    return 0;
}

void *kbuffer_get(kbuffer_t *buffer)
{
    size_t entry_size = (sizeof(kentry_t) + buffer->data_size);
    uint32_t base_ptr = buffer->ptr;
    kentry_t *entry;
    while (1)
    {
        entry = buffer->entries + (buffer->ptr++ * entry_size);

        if (!entry->used)
            return entry->data;

        if (buffer->ptr == buffer->length)
        {
            buffer->ptr = 0;
            continue;
        }

        if (buffer->ptr == base_ptr)
            return 0;
    }
}

void *kbuffer_drop(void *buffer)
{
    *((uint8_t *)buffer - 1) = 0;
}
