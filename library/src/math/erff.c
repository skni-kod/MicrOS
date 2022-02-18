#include "../math.h"

float erff(float x)
{
    return (float)erfl((long double)x);
}