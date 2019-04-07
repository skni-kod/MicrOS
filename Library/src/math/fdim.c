#include "../math.h"

double fdim(double x, double y)
{
    return x > y ? x - y : 0;
}