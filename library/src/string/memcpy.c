#include "../string.h"

void *memcpy(void *destination, const void *source, size_t size)
{
    uint64_t *long_destination_ptr = destination;
    const uint64_t *long_source_ptr = source;

    while(size >= 8)
    {
        *long_destination_ptr++ = *long_source_ptr++;
        size -= 8;
    }
    
    uint8_t *short_destination_ptr = (uint8_t *)long_destination_ptr;
    const uint8_t *short_source_ptr = (const uint8_t *)long_source_ptr;

    while(size > 0)
    {
        *short_destination_ptr++ = *short_source_ptr++;
        size--;
    }

    return destination;
}