#include "string.h"

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

int memcmp(const void *buffer1, const void *buffer2, size_t size)
{
    const uint8_t *ptr1 = buffer1;
    const uint8_t *ptr2 = buffer2;

    for (size_t i = 0; i < size; i++)
    {
        if (ptr1[i] != ptr2[i])
        {
            return ptr1[i] > ptr2[i] ? 1 : -1;
        }
    }

    return 0;
}

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

size_t strlen(const char *str)
{
    size_t length = 0;

    while (str[length] != 0)
    {
        length++;
    }

    return length;
}