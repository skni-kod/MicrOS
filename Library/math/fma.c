#include "../math.h"

double fma(double x, double y, double z)
{
    __asm__ (
        "fldl %3 \n" \
        "fldl %2 \n" \
        "fldl %1 \n" \
        "fmulp \n" \
        "faddp \n" \
        "fstpl %0"
        : "=m"(x): "m"(x), "m"(y), "m"(z));
    return x;
}