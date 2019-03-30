#include "../math.h"

float ldexpf(float x, int exp)
{
    __asm__ (
        "fild %2 \n" \
        "fld %1 \n" \
        "fscale \n" \
        "fstp %0"
        : "=m"(x): "m"(x), "m"(exp));
    return x;
}