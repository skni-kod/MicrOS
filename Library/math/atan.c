#include "../math.h"

double atan(double x)
{
    __asm__ (
        "fldl  %1 \n" \
        "fld1 \n" \
        "fpatan \n" \
        "fstpl %0 \n"
        : "=m"(x): "m"(x));
    return x;
}