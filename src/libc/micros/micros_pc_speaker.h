#ifndef MICROS_PC_SPEAKER_H
#define MICROS_PC_SPEAKER_H

#include "micros_interrupts.h"

#ifdef __cplusplus
extern "C" {
#endif

//! Enables PC speaker and does BEEEEP
/*!
    Enables PC speaker and does BEEEEP with the specified frequency.
    \param frequency Sound frequency.
*/
void micros_pc_speaker_enable_sound(uint32_t frequency);

//! Disables PC speaker
/*!
    Disables PC speaker and stops doing BEEEEP.
*/
void micros_pc_speaker_disable_sound();

#ifdef __cplusplus
}
#endif

#endif