#include "../math.h"

float sqrtf(float x)
{
    __asm__ (
        "fld  %1 \n" \
        "fsqrt \n" \
        "fstp %0"
        : "=m"(x): "m"(x));
    return x;
}