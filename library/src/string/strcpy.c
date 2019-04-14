#include "../string.h"

char *strcpy(char *destination, const char *source)
{
    char *str = destination;
    do
    {
        *destination = *source;
        source++,destination++;
    }while(*source);
    return str;
}