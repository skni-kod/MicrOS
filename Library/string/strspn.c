#include "../string.h"

size_t strspn(const char *str1, const char *str2)
{
    size_t ret = 0;
    while(*str1)
    {
        if(*strchr(str2,*str1) == '\0')
            return ret;
         else
            str1++,ret++;
    }
    return ret;
}