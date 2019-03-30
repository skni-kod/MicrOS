#include "../math.h"

long double coshl(long double x)
{
    return (expl(x) + expl(-x)) / 2;
}