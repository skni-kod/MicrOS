#include "../stdio.h"

char *fgets(char *str, int num, FILE *stream)
{
    int i = 0;
    while(1)
    {
        char c = fgetc(stream);
        if (c == EOF)
        {
            return NULL;
        }
        
        str[i++] = c;
        
        if (c == '\n' || c == -1 || i >= num)
        {
            break;
        }
    }

    str[i] = 0;
    return str;
}