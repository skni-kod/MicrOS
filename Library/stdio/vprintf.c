#include <stdarg.h>
#include "../stdio.h"

int vprintf(const char* format, va_list arg)
{
    return vfprintf(stdout, format, arg);
}
