#include "../string.h"

void *memcpy(void *destination, const void *source, size_t size)
{
    uint8_t *destination_ptr = destination;
    const uint8_t *source_ptr = source;

    for (size_t i = 0; i < size; i++)
    {
        destination_ptr[i] = source_ptr[i];
    }

    return destination_ptr;
}