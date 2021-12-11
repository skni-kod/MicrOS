#include "../math.h"

double erf(double x)
{
    return (double)erfl((long double)x);
}