#include "../math.h"

double ldexp(double x, int exp)
{
    __asm__ (
        "fild %2 \n" \
        "fldl %1 \n" \
        "fscale \n" \
        "fstpl %0"
        : "=m"(x): "m"(x), "m"(exp));
    return x;
}