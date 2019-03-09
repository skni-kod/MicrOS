#include "../stdio.h"

int puts(const char *str)
{
    return fputs(str, stdout), fputs("\n", stdout);
}