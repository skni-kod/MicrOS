#include "../ctype.h"

int isgraph(int c)
{
    if(c >= 0x21 && c <= 0x7E)
        return 1;
    else
        return 0;
}
