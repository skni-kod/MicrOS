#include "../string.h"

void *memcpy(void *destination, const void *source, size_t size)
{
    char *destinatino_ptr = destination;
    char *source_ptr = (char *)source;

    for(size_t i = 0; i < size; i++)
    {
        destinatino_ptr[i] = source_ptr[i];
    }
    
    return destination;
}