#include "../math.h"

long double erfcl(long double x)
{
    feclearexcept(FE_UNDERFLOW);

    long double result = 1 - erfl(x);

    fexcept_t exceptions = __FPU_read_status_word();
    if(exceptions.underflow == 1)
    {
        if(_math_errhandling == MATH_ERRNO)
        {
            errno = ERANGE;
            feclearexcept(FE_UNDERFLOW);
        }
        return 0;
    }
    return result;
}