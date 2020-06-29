#include "../stdlib.h"

void* bsearch(const void* key, const void* base, size_t num, size_t size, int (*compar)(const void*,const void*))
{
    const uint8_t *begin = base;
    for(int i = 0; i < num; ++i)
    {
        if(compar(key, begin + (i * size)) == 0)
        {
            return begin + (i * size);
        }
    }
    return NULL;
}