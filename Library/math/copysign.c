#include "../math.h"

double copysign(double x, double y)
{
    unsigned char sign_of_x = signbit(x);
    unsigned char sign_of_y = signbit(y);
    double * p_x = &x;
    unsigned char * ucharp_x = (unsigned char *)p_x;
    if(sign_of_x != sign_of_y)
    {
        if(sign_of_x == 0)
        {
            ucharp_x[sizeof(double) - 1] = ucharp_x[sizeof(double) - 1] | 128;
        }
        else
        {
            ucharp_x[sizeof(double) - 1] = ucharp_x[sizeof(double) - 1] & 127;
        }
    }
    return x;
}