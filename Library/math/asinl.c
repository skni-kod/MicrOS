#include "../math.h"

long double asinl(long double x)
{
    __asm__ (
        "fldt %1 \n" \
        "fldt %1 \n" \
        "fmulp \n" \
        "fld1 \n" \
        "fsubp \n" \
        "fsqrt \n" \
        "fldt %1 \n" \
        "fdivp \n" \
        "fld1 \n" \
        "fpatan \n" \
        "fstpt %0"
        : "=m"(x): "m"(x));
    return x;
}