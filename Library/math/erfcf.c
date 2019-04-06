#include "../math.h"

float erfcf(float x)
{
    feclearexcept(FE_UNDERFLOW);

    float result = 1 - erff(x);

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