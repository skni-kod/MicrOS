#include "../stdlib.h"


long int labs(long int n)
{
    if (n < 0)
    {
        n = n * -1;
    }
    return n;
}