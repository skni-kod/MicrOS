#include "../stdio.h"
#include <stdarg.h>

int fprintf(FILE *file, const char *format, ...)
{
    va_list arg;
    va_start(arg, format);

    int ret = vfprintf(file, format, arg);
    va_end(arg);

    return ret;
}