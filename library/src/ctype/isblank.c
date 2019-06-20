#include "../ctype.h"

int isblank(int c)
{
    if((c == 0x09 || c == 0x20))
        return 1;
    else
        return 0;
}
