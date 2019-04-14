#include "../math.h"

double modf(double x, double* iptr)
{
    double remainder;
    *iptr = trunc(x);
    remainder = x - *iptr;
    return remainder;
}