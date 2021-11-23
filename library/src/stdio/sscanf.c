#include <stdarg.h>
#include "../stdio.h"

int sscanf(const char *str, const char *format, ...)
{
    va_list arg;
    va_start(arg, format);

    FILE *stream = __stdio_create_stream();
    setvbuf(stream, str, _IOFBF, 0);

    int ret = vfscanf(stream, format, arg);

    va_end(arg);

    return ret;
}