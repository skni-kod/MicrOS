#include "../string.h"

char *strcat(char *destination, const char *source)
{
    char *ret = destination;
    while(*destination)
        destination++;
    do 
    {
        *destination = *source;
        destination++,source++;
    }while(*source);
    *destination = '\0';
    return ret;
        
}