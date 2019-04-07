#include "pc_speaker_calls.h"

void syscall_pc_speaker_enable_sound(interrupt_state *state)
{
    pc_speaker_enable_sound(state->registers.ebx);
}

void syscall_pc_speaker_disable_sound()
{
    pc_speaker_disable_sound();
}