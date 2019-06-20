#include "../ctype.h"

int ispunct(int c)
{
    if( (c >= 0x21 && c <= 0x2F) || (c >= 0x3a && c <= 0x40) || (c >= 0x5b && c <= 0x60) || (c >= 0x7b && c <= 0x7e))
        return 1;
    else
        return 0;
}
