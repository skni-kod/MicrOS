#include "../string.h"

void *memmove(void *destination, const void *source, size_t size)
{
    uint8_t *destination_ptr = destination;
    const uint8_t *source_ptr = source;

    if (destination_ptr < source_ptr)
    {
        memcpy(destination, source, size);
    }
    else
    {
        for (size_t i = size; i != 0; i--)
        {
            destination_ptr[i - 1] = source_ptr[i - 1];
        }
    }

    return destination;
}