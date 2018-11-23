#include "time.h"

clock_t clock()
{
    return call_interrupt_0a(0x01);
}