#include "../math.h"

float rintf(float x)
{
    __asm__ (
        "fld %1 \n" \
        "frndint \n" \
        "fstp %0"
        : "=m"(x): "m"(x));
    return x;
}