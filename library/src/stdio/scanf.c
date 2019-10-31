#include <stdarg.h>
#include "../stdio.h"

int scanf(const char *format, ...)
{
    va_list arg;
    va_start(arg, format);

    int ret = vfscanf(stdin, format, arg);

    va_end(arg);

    return ret;
}