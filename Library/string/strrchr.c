#include "../string.h"

char *strrchr(char *str, int character)
{
    char *ret = '\0';
    do
    {
        if(*str == (char)character)
            ret = str;
        str++;
    } while (*str);

    return ret; 
}