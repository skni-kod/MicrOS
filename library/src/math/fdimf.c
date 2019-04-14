#include "../math.h"

float fdimf(float x, float y)
{
    return x > y ? x - y : 0;
}