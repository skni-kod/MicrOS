#include "../math.h"

int __math_signbitl(long double x)
{
    long_double_byte fb;
    fb.f = x;

    int value = fb.bytes[9] & 128;
    return value;
}