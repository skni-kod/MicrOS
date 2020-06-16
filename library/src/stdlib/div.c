#include "../stdlib.h"


div_t div(int numer, int denom)
{
    div_t result = {numer / denom, numer % denom};
    return result;
}