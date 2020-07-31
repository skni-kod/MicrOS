#include "../stdlib.h"

void abort()
{
    micros_process_exit(-1, false);
}