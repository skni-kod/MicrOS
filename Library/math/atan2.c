#include "../math.h"

double atan2(double x, double y)
{
    __asm__ (
        "fldl %1 \n" \
        "fldl %2 \n" \
        "fpatan \n" \
        "fstpl %0 \n"
        : "=m"(x): "m"(x), "m"(y));
    return x;
}