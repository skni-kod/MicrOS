#include "../math.h"

float log2f(float x)
{
    __asm__ (
        "fld1 \n" \
        "fld %1 \n" \
        "fyl2x \n" \
        "fstp %0 \n"
        : "=m"(x): "m"(x));
    return x;
}