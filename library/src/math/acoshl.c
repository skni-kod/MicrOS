#include "../math.h"

long double acoshl(long double x)
{
    if(x < 1)
    {
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
    return logl(x + sqrtl(x * x - 1));
}