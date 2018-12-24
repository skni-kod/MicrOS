#include "time.h"

clock_t clock()
{
    return micros_get_system_clock();
}