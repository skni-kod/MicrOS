#include "../string.h"

char *strchr(char *str, int character)
{
    char *ret;
    while(*str != '\0' || *str == character)
        str++;
    return ret = str;
}