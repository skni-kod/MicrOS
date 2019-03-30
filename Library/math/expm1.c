#include "../math.h"

double expm1(double x)
{
    return pow(M_E, x) - 1;
}