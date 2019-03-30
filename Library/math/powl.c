#include "../math.h"

long double powl(long double base, long double exponent)
{
    // Use 2^(y*log2(x)) 
    // Compute y*log2(x)
    long double ex;
    __asm__ (
        "fldt %2 \n" \
        "fldt %1 \n" \
        "fyl2x \n" \
        "fstpt %0"
        : "=m"(ex): "m"(base), "m"(exponent));

    // Scale
    long double integer, remainder;
    remainder = modfl(ex, &integer);

    // Compute 2^ex
    long double resultBeforeScale;
    __asm__ (
        "fldt %1 \n" \
        "f2xm1 \n" \
        "fld1 \n" \
        "faddp \n" \
        "fstpt %0"
        : "=m"(resultBeforeScale): "m"(remainder));
    
    // Result after scale
    long double result;
    __asm__ (
        "fldt %2 \n" \
        "fldt %1 \n" \
        "fscale \n" \
        "fstpt %0"
        : "=m"(result): "m"(resultBeforeScale), "m"(integer));
    return result;  
}