#include "../ctype.h"

int isdigit(int c)
{
    if(c >= 0x30 && c <= 0x39)
        return 1;
    else
        return 0;
}
