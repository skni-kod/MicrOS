#include "../math.h"

float cbrtf(float x)
{
    float result = x;
    float one_over_three = 1.0 / 3.0;
    int i;
    for(i = 0; i < 9; i++)
    {
        result = one_over_three * (x / powf(result, 2) + 2 * result);
    }
    return result;
}