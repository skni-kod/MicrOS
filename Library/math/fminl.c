#include "../math.h"

long double fminl(long double x, long double y)
{
    return x < y ? x : y;
}