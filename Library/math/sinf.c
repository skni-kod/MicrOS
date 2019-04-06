#include "../math.h"

float sinf(float x)
{
    __asm__ (
        "fld %1 \n" \
        "fsin \n" \
        "fstp %0"
        : "=m"(x): "m"(x));
    return x;
}