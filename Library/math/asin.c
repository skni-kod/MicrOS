#include "../math.h"

double asin(double x)
{
    __asm__ (
        "fldl %1 \n" \
        "fldl %1 \n" \
        "fmulp \n" \
        "fld1 \n" \
        "fsubp \n" \
        "fsqrt \n" \
        "fldl %1 \n" \
        "fdivp \n" \
        "fld1 \n" \
        "fpatan \n" \
        "fstpl %0"
        : "=m"(x): "m"(x));
    return x;
}