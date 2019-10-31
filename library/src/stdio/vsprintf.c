#include <stdarg.h>
#include "../stdio.h"

int vsprintf(char *str, const char *format, va_list arg)
{
    FILE *stream = __stdio_create_stream();
    setvbuf(stream, str, _IOFBF, 0);

    int ret = vfprintf(stream, format, arg) + 1;

    str[ret] = '\0';

    return ret;
}