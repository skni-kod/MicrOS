#include "../string.h"

size_t strcspn(const char *str1, const char *str2)
{
    size_t ret = 0;
    while(*str1)
    {
        if(*strchr(str2,*str1))
            return ret;
         else
            str1++,ret++;
    }
    return ret;
}