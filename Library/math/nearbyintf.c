#include "../math.h"

float nearbyintf(float x)
{
    __asm__ (
        "fld %1 \n" \
        "frndint \n" \
        "fstp %0"
        : "=m"(x): "m"(x));
    return x;
}