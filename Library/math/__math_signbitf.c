#include "../math.h"

int __math_signbitf(float x)
{
    float_byte fb;
    fb.f = x;

    int value = fb.bytes[3] & 128;
    return value;
}