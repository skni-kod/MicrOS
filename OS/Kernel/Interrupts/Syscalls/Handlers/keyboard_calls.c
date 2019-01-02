#include "keyboard_calls.h"

void syscall_keyboard_is_key_pressed(interrupt_state *state)
{
    state->registers.eax = !keyboard_is_buffer_empty();
}

void syscall_keyboard_get_pressed_key(interrupt_state *state)
{
    state->registers.eax = keyboard_get_key_from_buffer((keyboard_scan_ascii_pair *)state->registers.ebx);
}