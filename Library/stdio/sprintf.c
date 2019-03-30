#include "../stdio.h"
#include <stdarg.h>

int sprintf(char *str, const char *format, ...)
{
    va_list arg;
    va_start(arg, format);

    FILE *stream = __stdio_create_stream();
    setvbuf(stream, str, _IOFBF, 0);

    int ret = vfprintf(stream, format, arg) + 1;

    str[ret] = '\0';

    va_end(arg);

    return ret;
}