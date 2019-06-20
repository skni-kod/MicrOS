#include "../ctype.h"

int isprint(int c)
{
    if( c >= 0x20 && c <= 0x7E)
        return 1;
    else
        return 0;
}
