#include "../math.h"

long double __math_INFl()
{
    long_double_byte f;
    f.bytes[sizeof(long double) - 1] = 0x00;
    f.bytes[sizeof(long double) - 2] = 0x00;
    f.bytes[sizeof(long double) - 3] = 0x7F;
    f.bytes[sizeof(long double) - 4] = 0xFF;
    f.bytes[sizeof(long double) - 5] = 0x80;
    for(int i = sizeof(long double) - 6; i >= 0; --i)
    {
        f.bytes[i] = 0x00;
    }
    return f.f;
}