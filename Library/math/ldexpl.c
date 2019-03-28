#include "../math.h"

long double ldexpl(long double x, int exp)
{
    long double result;
    __asm__ (
        "fild %2 \n" \
        "fldt %1 \n" \
        "fscale \n" \
        "fstpt %0"
        : "=m"(result): "m"(x), "m"(exp));
    fexcept_t exceptions = __FPU_read_status_word();
    if(exceptions.overflow == 0)
    {
        return result;
    }
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