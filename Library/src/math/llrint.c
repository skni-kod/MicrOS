#include "../math.h"

long long int llrint(double x)
{
    long long int result;
    __asm__ (
        "fldl %1 \n" \
        "frndint \n" \
        "fistp %0"
        : "=m"(result): "m"(x));
    return result;
}