#include "../math.h"

float asinf(float x)
{
    __asm__ (
        "fld %1 \n" \
        "fld %1 \n" \
        "fmulp \n" \
        "fld1 \n" \
        "fsubp \n" \
        "fsqrt \n" \
        "fld %1 \n" \
        "fdivp \n" \
        "fld1 \n" \
        "fpatan \n" \
        "fstp %0"
        : "=m"(x): "m"(x));
    return x;
}