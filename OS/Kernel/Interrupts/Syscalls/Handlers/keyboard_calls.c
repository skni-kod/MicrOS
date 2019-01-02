#include "keyboard_calls.h"

void syscall_keyboard_is_key_pressed(interrupt_state *state)
{
    state->registers.eax = !keyboard_is_buffer_empty();
}