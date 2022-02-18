#include "../math.h"

long double erfl(long double x)
{
    // Make it faster for obivious values
    if(x == 0)
    {
        return 0;
    }
    if(x > 3.56)
    {
        return 1;
    }
    if(x < -3.56)
    {
        return -1;
    }

    long double sum = 0;
    long double span = 0.000000001;
    long double current_piece = 0;

    for(current_piece; current_piece < x; current_piece += span)
	{
		sum += exp(-pow(current_piece, 2)) * span;
	}
	sum = sum * (2.0/sqrt(M_PI));

    // Mke sure result isn't higher than 1.0
    if(sum > 1.0)
    {
        sum = 1.0;
    }
    // If argument was negative, neagte result
    if(x < 0)
    {
        sum *= -1;
    }

    return sum;
}