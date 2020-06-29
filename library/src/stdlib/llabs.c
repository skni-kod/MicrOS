#include "../stdlib.h"


long long int llabs(long long int n)
{
    if (n < 0)
    {
        n = n * -1;
    }
    return n;
}