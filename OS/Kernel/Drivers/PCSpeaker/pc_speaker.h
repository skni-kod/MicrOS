#ifndef PC_SPEAKER_H
#define PC_SPEAKER_H

#include <stdint.h>

void pc_speaker_enable_sound(uint32_t freq);
void pc_speaker_disable_sound();

#endif