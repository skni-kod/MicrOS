#include "../stdlib.h"

extern unsigned int seed;

void srand(unsigned int new_seed)
{
    seed = new_seed;
}