#include "../math.h"

long long int llrintf(float x)
{
    long long int result;
    __asm__ (
        "fld %1 \n" \
        "frndint \n" \
        "fistp %0"
        : "=m"(result): "m"(x));
    return result;
}