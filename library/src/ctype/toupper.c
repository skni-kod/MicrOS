#include "../ctype.h"

int toupper(int c)
{
	if( c>=0x61 && c<=0x7a)
        return (c = c + 0x41 - 0x61);
    else
        return c;
}
