#include "stdio.h"
#include "stdarg.h"
#include "../kernel_stdio.h"

int kernel_sprintf(char *str, const char *format, ...)
{
    va_list arg;
    va_start(arg, format);

    FILE *stream = __kernel_stdio_create_stream();
    kernel_setvbuf(stream, str, _IOFBF, 0);

    int ret = kernel_vfprintf(stream, format, arg) + 1;

    str[ret] = '\0';

    va_end(arg);

    return ret;
}