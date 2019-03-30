#include "../math.h"

long double sinhl(long double x)
{
    return (expl(x) - expl(-x)) / 2;
}