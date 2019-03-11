#include "../math.h"

double log2(double x)
{
    __asm__ (
        "fld1 \n" \
        "fldl %1 \n" \
        "fyl2x \n" \
        "fstpl %0 \n"
        : "=m"(x): "m"(x));
    return x;
}