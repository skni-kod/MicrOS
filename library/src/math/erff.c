#include "../math.h"

float erff(float x)
{
    if(x == 0)
    {
        return 0;
    }
    float sign;
    float factorial_of_j = 1;
    float sum = 0;
    for(int i = 1, j = 0; i < 22; i+=2, j++)
    {
        if(j % 2 == 0)
        {
            sign = 1;
        }
        else
        {
            sign = -1;
        }
        // Calculate factorial od j
        if(j > 0)
        {
            factorial_of_j *= j;
        }
        sum = sum + (sign * powf(x, (float)i)/((float)i * factorial_of_j));   
    }
    sum = sum * 2/sqrt(M_PI);
    return sum;
}