#include "../math.h"

long long int llrintl(long double x)
{
    long long int result;
    __asm__ (
        "fldt %1 \n" \
        "frndint \n" \
        "fistp %0"
        : "=m"(result): "m"(x));
    return result;
}