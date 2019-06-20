#include "../ctype.h"

int isxdigit(int c)
{
    if( (c >= 0x61 && c <= 0x66) || (c >= 0x41 && c <= 0x46) || (c >= 0x30 && c <= 0x39))
    return 1;
    else
        return 0;
}
