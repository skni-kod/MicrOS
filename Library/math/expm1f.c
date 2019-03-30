#include "../math.h"

float expm1f(float x)
{
    return powf(M_E, x) - 1;
}