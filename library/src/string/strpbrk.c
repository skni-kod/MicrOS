#include "../string.h"

char *strpbrk(char *str1, const char *str2)
{
    char *ret = (char *)str2;
    while(*str1 != '\0')
    {
        while(*str2 != '\0')
        {
            if(*str1 == *str2)
                return ret = str1;
            str2++;
        }
        str2 = ret;
        str1++;
    }
    return ret = str1;
}
