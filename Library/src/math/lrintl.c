#include "../math.h"

long int lrintl(long double x)
{
    long int result;
    __asm__ (
        "fldt %1 \n" \
        "frndint \n" \
        "fistp %0"
        : "=m"(result): "m"(x));
    return result;
}