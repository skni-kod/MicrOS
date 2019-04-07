#include "../math.h"

int __math_signbit(double x)
{
    double_byte fb;
    fb.f = x;

    int value = fb.bytes[7] & 128;
    return value;
}