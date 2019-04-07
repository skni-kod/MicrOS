#include "../stdio.h"

char *gets(char *str)
{
    return fgets(str, INT32_MAX, stdin);
}