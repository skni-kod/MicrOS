#include "../math.h"

float modff(float x, float* iptr)
{
    float remainder;
    *iptr = truncf(x);
    remainder = x - *iptr;
    return remainder;
}