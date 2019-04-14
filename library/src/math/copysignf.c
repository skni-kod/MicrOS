#include "../math.h"

float copysignf(float x, float y)
{
    unsigned char sign_of_x = signbit(x);
    unsigned char sign_of_y = signbit(y);
    float * p_x = &x;
    unsigned char * ucharp_x = (unsigned char *)p_x;
    if(sign_of_x != sign_of_y)
    {
        if(sign_of_x == 0)
        {
            ucharp_x[sizeof(float) - 1] = ucharp_x[sizeof(float) - 1] | 128;
        }
        else
        {
            ucharp_x[sizeof(float) - 1] = ucharp_x[sizeof(float) - 1] & 127;
        }
    }
    return x;
}