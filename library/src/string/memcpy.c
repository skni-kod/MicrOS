#include "../string.h"

void *memcpy(void *destination, const void *source, size_t size)
{
    uint64_t *destination_64ptr = (uint64_t *)destination;
    uint64_t *source_64ptr = (uint64_t *)source;
    while (size >= 8)
    {
        *destination_64ptr++ = *source_64ptr++;
        size -= 8;
    }
    
    uint32_t *destination_32ptr = (uint32_t *)destination_64ptr;
    uint32_t *source_32ptr = (uint32_t *)source_64ptr;
    while (size >= 4)
    {
        *destination_32ptr++ = *source_32ptr++;
        size -= 4;
    }
     
    uint8_t *destination_ptr = (uint8_t *)destination_32ptr;
    uint8_t *source_ptr = (uint8_t *)source_32ptr;
    while (size >= 1)
    {
        *destination_ptr++ = *source_ptr++;
        size--;
    }
    
    return destination;
}