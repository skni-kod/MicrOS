#include "../math.h"

double scalbln(double x, long int n)
{
    feclearexcept(FE_OVERFLOW);
    feclearexcept(FE_UNDERFLOW);
    
    // This should be FLX_RADIX based but, the value is 2 on all machines we know of except the IBM 360 and derivatives.
    // So we just could call ldexp, but it takes int instead of long int so we can copy asm.
    __asm__ (
        "fild %2 \n" \
        "fldl %1 \n" \
        "fscale \n" \
        "fstpl %0"
        : "=m"(x): "m"(x), "m"(n));
    fexcept_t exceptions = __FPU_read_status_word();
    if(exceptions.overflow == 0 && exceptions.underflow == 0)
    {
        return (int)x;
    }
    // range error
    else if(exceptions.overflow == 1)
    {
        if(_math_errhandling == MATH_ERRNO)
        {
            errno = ERANGE;
            feclearexcept(FE_OVERFLOW);
        }
        // In other case overflow is already set so we don't need to cover _math_errhandling == MATH_ERREXCEPT
        return x > 0 ? HUGE_VAL : copysign(HUGE_VAL, -1.0);
    }
    // range error (exceptions.underflow == 1)
    else
    {
        if(_math_errhandling == MATH_ERRNO)
        {
            errno = ERANGE;
            feclearexcept(FE_UNDERFLOW);
        }
        return 0;
    }
}