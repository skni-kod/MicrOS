#include "../math.h"

long double atanhl(long double x)
{
    return 0.5 * logl((1 + x)/(1 - x));
}