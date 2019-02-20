#ifndef MICROS_PC_SPEAKER_H
#define MICROS_PC_SPEAKER_H

#include "micros_interrupts.h"

void micros_pc_speaker_enable_sound(uint32_t frequency);
void micros_pc_speaker_disable_sound();

#endif