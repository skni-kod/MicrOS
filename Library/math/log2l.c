#include "../math.h"

long double log2l(long double x)
{
    __asm__ (
        "fld1 \n" \
        "fldt %1 \n" \
        "fyl2x \n" \
        "fstpt %0 \n"
        : "=m"(x): "m"(x));
    return x;
}