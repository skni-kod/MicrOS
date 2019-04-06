#include "../math.h"

double nearbyint(double x)
{
    __asm__ (
        "fldl %1 \n" \
        "frndint \n" \
        "fstpl %0"
        : "=m"(x): "m"(x));
    return x;
}