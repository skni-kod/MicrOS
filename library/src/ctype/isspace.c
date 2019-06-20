#include "../ctype.h"

int isspace(int c)
{
    if((c > 0x09 && c <=0x0D) || c == 0x20)
        return 1;
    else
        return 0;
}
