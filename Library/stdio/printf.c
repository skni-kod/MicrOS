#include "../stdio.h"
#include <stdarg.h>

int printf(const char *format, ...)
{
    va_list arg;
    va_start(arg, format);

    int ret = vfprintf(stdout, format, arg);
    va_end(arg);

    return ret;
}