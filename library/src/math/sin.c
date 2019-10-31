#include "../math.h"

double sin(double x)
{
    __asm__ (
        "fldl %1 \n" \
        "fsin \n" \
        "fstpl %0"
        : "=m"(x): "m"(x));
    return x;
}
