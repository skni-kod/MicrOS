#include "../math.h"

long double copysignl(long double x, long double y)
{
    unsigned char sign_of_x = signbit(x);
    unsigned char sign_of_y = signbit(y);
    long double * p_x = &x;
    unsigned char * ucharp_x = (unsigned char *)p_x;
    if(sign_of_x != sign_of_y)
    {
        if(sign_of_x == 0)
        {
            ucharp_x[sizeof(long double) - 3] = ucharp_x[sizeof(long double) - 3] | 128;
        }
        else
        {
            ucharp_x[sizeof(long double) - 3] = ucharp_x[sizeof(long double) - 3] & 127;
        }
    }
    return x;
}