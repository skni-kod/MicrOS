#include "../math.h"

long double atanhl(long double x)
{
    if(x > 1 || x < -1)
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
    if(x == 1 || x == -1)
    {
        if(_math_errhandling == MATH_ERRNO)
        {
            errno = ERANGE;
        }
        else if(_math_errhandling == MATH_ERREXCEPT)
        {
            feraiseexcept(FE_DIVBYZERO);  
        }
        return x > 0 ? __math_INFl() : -__math_INFl();
    }
    return 0.5 * logl((1 + x)/(1 - x));
}