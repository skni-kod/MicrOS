#include "../ctype.h"

int isalnum(int c)
{
    if((c >= 0x61 && c <= 0x7a) || (c >= 0x41 && c <= 0x5a) || (c >= 0x30 && c <= 0x39))
        return 1;
    else
        return 0;
}
