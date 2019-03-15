#include "../string.h"

char *strncpy(char *destination, const char *source, size_t num)
{
    char *str = destination;
    do
    {
        *destination = *source;
        source++,destination++,num--;
    }while(*source && num);
    return str;
}