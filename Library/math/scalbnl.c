#include "../math.h"

long double scalbnl(long double x, int n)
{
    feclearexcept(FE_OVERFLOW);
    feclearexcept(FE_UNDERFLOW);
    
    // This should be FLX_RADIX based but, the value is 2 on all machines we know of except the IBM 360 and derivatives.
    // So we just call ldexpl.
    long double result = ldexpl(x, n);
    fexcept_t exceptions = __FPU_read_status_word();
    if(exceptions.overflow == 0 && exceptions.underflow == 0)
    {
        return (int)result;
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
        return x > 0 ? HUGE_VALL : copysignl(HUGE_VALL, -1.0);
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