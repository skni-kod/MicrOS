#include "../ctype.h"

int isalpha(int c)
{
    if((c >= 0x61 && c <= 0x7a) || (c >= 0x41 && c <= 0x5a))
        return 1;
    else
        return 0;
}
