#include "../ctype.h"

int tolower(int c)
{
	if( c>=0x41 && c<=0x5a)
        return (c = c + 0x61 - 0x41);
    else
        return c;
}
