#include "../string.h"

void *memset(void *buffer, int value, size_t size)
{
    uint8_t *ptr = (uint8_t *)buffer;
    for (size_t i = 0; i < size; i++)
    {
        *ptr = value;
        ptr++;
    }

    return buffer;
}