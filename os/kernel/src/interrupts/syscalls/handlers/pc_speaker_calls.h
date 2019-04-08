#ifndef PC_SPEAKER_CALLS_H
#define PC_SPEAKER_CALLS_H

#include "interrupts/idt/interrupt_state.h"
#include "Drivers/PCSpeaker/pc_speaker.h"

void syscall_pc_speaker_enable_sound(interrupt_state *state);
void syscall_pc_speaker_disable_sound();

#endif