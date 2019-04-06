#include "../math.h"

float exp2f(float x)
{
    feclearexcept(FE_OVERFLOW);

    float result = powf(2, x);
    
    fexcept_t exceptions = __FPU_read_status_word();
    if(exceptions.overflow == 0)
    {
        return result;
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
        return x > 0 ? HUGE_VALF : copysignf(HUGE_VALF, -1.0);
    }
}