#include "../stdlib.h"

extern unsigned int seed;

int rand()
{
    const int multiplier = 1103515245;
    const int increment = 12345;
    const int modulus = 2147483648;

    if (seed == 0)
    {
        seed = time(NULL);
    }

    seed = (multiplier * seed + increment) % modulus;
    return seed;
}