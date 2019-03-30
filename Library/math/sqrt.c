#include "../math.h"

double sqrt(double x)
{
    __asm__ (
        "fldl  %1 \n" \
        "fsqrt \n" \
        "fstpl %0"
        : "=m"(x): "m"(x));
    return x;  
}