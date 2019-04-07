#include "../string.h"

char *strtok(char *str, const char *delimiters)
{
    static char* p=0;
    if(str)
        p=str;
    else if(!p)
        return 0;
    str=p+strspn(p,delimiters);
    p=str+strcspn(str,delimiters);
    if(p==str)
        return p=0;
    p = *p ? *p=0,p+1 : 0;
    return str;
}