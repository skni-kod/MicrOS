#include "../math.h"

long double expm1l(long double x)
{
    return powl(M_E, x) - 1;
}