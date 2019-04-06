#include "../math.h"

float powf(float base, float exponent)
{
    feclearexcept(FE_OVERFLOW);
    feclearexcept(FE_UNDERFLOW);
    
    if(base < 0 && ceilf(exponent) == exponent && floorf(exponent) == exponent)
    {
        //domain error
        if(_math_errhandling == MATH_ERRNO)
        {
            errno = EDOM;
        }
        else if(_math_errhandling == MATH_ERREXCEPT)
        {
            feraiseexcept(FE_INVALID);  
        }
        return 0;
    }
    if(base == 0 && exponent == 0)
    {
        //domain error
        if(_math_errhandling == MATH_ERRNO)
        {
            errno = EDOM;
        }
        else if(_math_errhandling == MATH_ERREXCEPT)
        {
            feraiseexcept(FE_INVALID);  
        }
        return 0;
    }
    if(base == 0 && exponent < 0)
    {
        //domain error
        if(_math_errhandling == MATH_ERRNO)
        {
            errno = EDOM;
        }
        else if(_math_errhandling == MATH_ERREXCEPT)
        {
            feraiseexcept(FE_INVALID);  
        }
        return 0;
    }
    
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
    remainder = modff(ex, &integer);

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

    fexcept_t exceptions = __FPU_read_status_word();
    // range error
    if(exceptions.overflow == 1 || exceptions.underflow == 1)
    {
        if(_math_errhandling == MATH_ERRNO)
        {
            errno = ERANGE;
            feclearexcept(FE_OVERFLOW);
            feclearexcept(FE_UNDERFLOW);
        }
        // In other case overflow or underflow is already set so we don't need to cover _math_errhandling == MATH_ERREXCEPT
        return 0;
    }
    return result; 
}