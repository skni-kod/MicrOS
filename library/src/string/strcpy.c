#include "../string.h"

char *strcpy(char *destination, const char *source)
{
    while(*source)
        *(destination++) = *(source++);
    *destination = 0;    
    return destination;
}