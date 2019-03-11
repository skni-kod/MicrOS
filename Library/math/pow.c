#include "../math.h"

double pow(double base, double exponent)
{
    // Use 2^(y*log2(x)) 
    // Compute y*log2(x)
    double ex;
    __asm__ (
        "fldl %2 \n" \
        "fldl %1 \n" \
        "fyl2x \n" \
        "fstpl %0"
        : "=m"(ex): "m"(base), "m"(exponent));

    // Scale
    double integer, remainder;
    remainder = modf(ex, &integer);

    // Compute 2^ex
    double resultBeforeScale;
    __asm__ (
        "fldl %1 \n" \
        "f2xm1 \n" \
        "fld1 \n" \
        "faddp \n" \
        "fstpl %0"
        : "=m"(resultBeforeScale): "m"(remainder));
    
    // Result after scale
    double result;
    __asm__ (
        "fldl %2 \n" \
        "fldl %1 \n" \
        "fscale \n" \
        "fstpl %0"
        : "=m"(result): "m"(resultBeforeScale), "m"(integer));
    return result;  
}