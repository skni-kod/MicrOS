#include "../math.h"

double tan(double x)
{
    __asm__ (
        "fldl %1 \n" \
        "fptan \n" \
        "fstpl %0 \n" \
        "fstpl %0"
        : "=m"(x): "m"(x));
    return x;
}