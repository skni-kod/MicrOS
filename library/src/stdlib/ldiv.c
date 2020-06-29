#include "../stdlib.h"


ldiv_t ldiv(long int numer, long int denom)
{
    ldiv_t result = {numer / denom, numer % denom};
    return result;
}