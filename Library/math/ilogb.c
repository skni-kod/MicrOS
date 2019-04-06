#include "../math.h"

int ilogb(double x)
{
    feclearexcept(FE_OVERFLOW);
    
    if(x == 0)
    {
        // domain error
        if(_math_errhandling == MATH_ERRNO)
        {
            errno = EDOM;
        }
        else if(_math_errhandling == MATH_ERREXCEPT)
        {
            feraiseexcept(FE_INVALID);  
        }
        return FP_ILOGB0;
    }
    else if(isnan(x) == 1)
    {
        // domain error
        if(_math_errhandling == MATH_ERRNO)
        {
            errno = EDOM;
        }
        else if(_math_errhandling == MATH_ERREXCEPT)
        {
            feraiseexcept(FE_INVALID);  
        }
        return FP_ILOGBNAN;
    }
    else if(isinf(x) == 1)
    {
        // domain error
        if(_math_errhandling == MATH_ERRNO)
        {
            errno = EDOM;
        }
        else if(_math_errhandling == MATH_ERREXCEPT)
        {
            feraiseexcept(FE_INVALID);  
        }
        return INT_MAX;
    }
    double exponent = 0;
    __asm__ (
        "fldl %2 \n" \
        "fxtract \n" \
        "fstpl %1 \n" \
        "fstpl %0"
        : "=m"(exponent), "=m"(x): "m"(x));
    fexcept_t exceptions = __FPU_read_status_word();
    if(exceptions.overflow == 0)
    {
        return (int)exponent;
    }
    // range error
    else
    {
        if(_math_errhandling == MATH_ERRNO)
        {
            errno = ERANGE;
            feclearexcept(FE_OVERFLOW);
        }
        // In other case overflow is already set so we don't need to cover _math_errhandling == MATH_ERREXCEPT
        return x > 0 ? HUGE_VAL : copysign(HUGE_VAL, -1.0);
    }
}