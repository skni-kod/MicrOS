#include "../stdio.h"

int fputs(const char *str, FILE *stream)
{
    return fwrite(str, strlen(str), 1, stream);
}