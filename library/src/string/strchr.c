#include "../string.h"

char *strchr(const char *str, int character)
{
    while(*str != '\0' && *str != character)
        str++;
    return (char *)str;
}