#include "../math.h"

long double fdiml(long double x, long double y)
{
    return x > y ? x - y : 0;
}