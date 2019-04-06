#include "../math.h"

long double modfl(long double x, long double* iptr)
{
    long double remainder;
    *iptr = truncl(x);
    remainder = x - *iptr;
    return remainder;
}