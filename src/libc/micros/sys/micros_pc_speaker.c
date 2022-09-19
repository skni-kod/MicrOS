#include "micros_pc_speaker.h"

void micros_pc_speaker_enable_sound(uint32_t frequency)
{
#ifndef __libk 

    micros_interrupt_1a(0x0800, frequency);

#else

#endif
}

void micros_pc_speaker_disable_sound()
{
#ifndef __libk 

    micros_interrupt_0a(0x0801);

#else

#endif
}