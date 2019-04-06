#include "../math.h"

double atanh(double x)
{
    return 0.5 * log((1 + x)/(1 - x));
}