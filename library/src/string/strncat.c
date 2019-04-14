#include "../string.h"

char *strncat(char *destination, const char *source, size_t num)
{
    char *ret = destination;
    while(*destination)
        destination++;
    do 
    {
        *destination = *source;
        destination++,source++,num--;
    }while(*source && num);
    *destination = '\0';
    return ret;
}