#include "../math.h"

float sinhf(float x)
{
    return (expf(x) - expf(-x)) / 2;
}