#include "../math.h"

double fabs(double x)
{
    __asm__ (
        "fldl %1 \n" \
        "fabs \n " \
        "fstpl %0"
        : "=m"(x): "m"(x));
    return x;
}