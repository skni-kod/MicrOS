#include "micros_pc_speaker.h"

void micros_pc_speaker_enable_sound(uint32_t frequency)
{
    micros_interrupt_1a(0x0800, frequency);
}

void micros_pc_speaker_disable_sound()
{
    micros_interrupt_0a(0x0801);
}