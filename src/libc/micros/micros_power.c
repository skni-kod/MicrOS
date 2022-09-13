#include "micros_power.h"

void micros_power_reboot()
{
#ifndef __libk 

    micros_interrupt_0a(0x0C00);

#else

#endif
}

void micros_power_shutdown()
{
#ifndef __libk 

    micros_interrupt_0a(0x0C01);

#else

#endif
}