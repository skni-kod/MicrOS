#include "../string.h"

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