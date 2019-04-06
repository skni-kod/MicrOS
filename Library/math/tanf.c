#include "../math.h"

float tanf(float x)
{
    __asm__ (
        "fld %1 \n" \
        "fptan \n" \
        "fstp %0 \n" \
        "fstp %0"
        : "=m"(x): "m"(x));
    return x;
}