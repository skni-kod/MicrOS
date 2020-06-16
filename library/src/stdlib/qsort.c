#include "../stdlib.h"

void __stdlib_swap(void *first, void *second, size_t size)
{
    uint8_t *a = first;
    uint8_t *b = second;

    for(int i = 0; i < size; ++i)
    {
        uint8_t temp = *a;
        *a = *b;
        *b = temp;
        ++a;
        ++b;
    }
}

int __stdlib_partition(void* base, size_t num, size_t size, int (*compar)(const void *, const void*))
{
    uint8_t *key = base + (size * (num - 1));
    uint8_t *begin = base;
    // Represent first item from left that is larger than key
    uint32_t itemFromLeft = 0;
    // Represent first item from left that is smaller or equal than key
    uint32_t itemFromRight = num - 2;
    
    while(true)
    {
        while(compar(begin + (itemFromLeft * size), key) < 0)
        {
            // We're going too far. Probably no element is greather than key. So the key is the greathest, swap it.
            if(itemFromLeft == num - 1)
            {
                __stdlib_swap(begin + (itemFromLeft * size), key, size);
                return itemFromLeft;
            }
            ++itemFromLeft;
        }
        while(compar(begin + (itemFromRight * size), key) >= 0)
        {
            // We're going too far. Probably no element is lower or equal than key. So the key is the lowest, swap it.
            if(itemFromRight == 0)
            {
                __stdlib_swap(begin + (itemFromRight * size), key, size);
                return itemFromRight;
            }
            --itemFromRight;
        }
        if (itemFromLeft < itemFromRight)
        {
            __stdlib_swap(begin + (itemFromLeft * size), begin + (itemFromRight * size), size);
            ++itemFromLeft;
            --itemFromRight;
        }
        else
        {
            __stdlib_swap(begin + (itemFromLeft * size), key, size);
            return itemFromLeft;
        }
    }
}

void qsort(void *base, size_t num, size_t size, int (*compar)(const void *, const void*))
{
    int separator;
    if(num > 1)
    {
        separator = __stdlib_partition(base, num, size, compar);
        qsort(base, separator, size, compar);
        qsort(base + (size * (separator + 1)), num - separator - 1, size, compar);
    }
}