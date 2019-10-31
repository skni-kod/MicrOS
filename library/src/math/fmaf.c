#include "../math.h"

float fmaf(float x, float y, float z)
{
    __asm__ (
        "fld %3 \n" \
        "fld %2 \n" \
        "fld %1 \n" \
        "fmulp \n" \
        "faddp \n" \
        "fstp %0"
        : "=m"(x): "m"(x), "m"(y), "m"(z));
    return x;
}