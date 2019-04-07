#include "../math.h"

double cos(double x)
{
    __asm__ (
        "fldl %1 \n" \
        "fcos \n" \
        "fstpl %0"
        : "=m"(x): "m"(x));
    return x;
}