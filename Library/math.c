#include "math.h"

double fabs(double x)
{
    __asm__ ("fldl  %1 \n fabs \n fstl %0" : "=m"(x): "m"(x));
    return x;
}

float fabsf(float x)
{
    __asm__ ("fld  %1 \n fabs \n fst %0" : "=m"(x): "m"(x));
    return x;
}

long double fabsl(long double x)
{
    __asm__ ("fld %1 \n fabs \n fstp %0" : "=m"(x): "m"(x));
    return x;
}