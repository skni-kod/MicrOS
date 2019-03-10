#include "../math.h"

float powf(float base, float exponent)
{
    // Use 2^(y*log2(x)) 
    // Compute y*log2(x)
    float ex;
    __asm__ (
        "fld  %2 \n" \
        "fld  %1 \n" \
        "fyl2x \n" \
        "fstp %0"
        : "=m"(ex): "m"(base), "m"(exponent));

    // Scale
    float integer, remainder;
    integer = (int)ex;
    remainder = ex - integer;

    // Compute 2^ex
    float resultBeforeScale;
    __asm__ (
        "fld  %1 \n" \
        "f2xm1 \n" \
        "fld1 \n" \
        "faddp \n" \
        "fstp %0"
        : "=m"(resultBeforeScale): "m"(remainder));
    
    // Result after scale
    float result;
    __asm__ (
        "fld  %2 \n" \
        "fld  %1 \n" \
        "fscale \n" \
        "fstp %0"
        : "=m"(result): "m"(resultBeforeScale), "m"(integer));
    return result;  
}