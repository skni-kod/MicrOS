#include "../string.h"

void *memchr(void *ptr, int value, size_t num)
{
    unsigned char *p = (unsigned char*)ptr;
    while(num)
    {
        if(*p != (unsigned char)value)
            p++;
        else
            return p;
        num--;
    }
    return 0;
}