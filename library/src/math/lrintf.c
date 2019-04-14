#include "../math.h"

long int lrintf(float x)
{
    long int result;
    __asm__ (
        "fld %1 \n" \
        "frndint \n" \
        "fistp %0"
        : "=m"(result): "m"(x));
        return result;
}