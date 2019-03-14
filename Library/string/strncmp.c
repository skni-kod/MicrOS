#include "../string.h"

int strncmp(const char *str1, const char *str2, size_t num)
{
    while(num && *str1 && (*str1==*str2))
    {
        str1++,str2++;
        num--;
    }
    return *(const unsigned char*)str1-*(const unsigned char*)str2;
}
