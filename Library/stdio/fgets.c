#include "../stdio.h"

char *fgets(char *str, int num, FILE *stream)
{
    char c = fgetc(stream);
    int i = 0;

    do
    {
        str[i++] = c;
    } while ((c = fgetc(stream), c != '\n' && c != -1) && i < num);

    str[i] = 0;
    return str;
}