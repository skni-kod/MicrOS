#include "../stdlib.h"


lldiv_t lldiv(long long int numer, long long int denom)
{
    lldiv_t result = {numer / denom, numer % denom};
    return result;
}