#include "../ctype.h"

int isupper(int c)
{
    if(c >= 0x41 && c <= 0x5a)
        return 1;
    else
        return 0;
}
