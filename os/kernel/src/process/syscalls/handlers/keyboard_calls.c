#include "keyboard_calls.h"

void syscall_keyboard_is_key_pressed(interrupt_state *state)
{
    if(process_manager_is_current_process_active())
    {
        state->registers.eax = !keyboard_is_buffer_empty();
    }
    else
    {
        state->registers.eax = false;
    }
}

void syscall_keyboard_get_pressed_key(interrupt_state *state)
{
    if(process_manager_is_current_process_active())
    {
        state->registers.eax = keyboard_get_key_from_buffer((keyboard_scan_ascii_pair *)state->registers.ebx);
    }
    else
    {
        state->registers.eax = false;
    }
}

void syscall_keyboard_wait_for_key_press(interrupt_state *state)
{
    process_manager_current_process_wait_for_key_press();
}

void syscall_keyboard_get_key_state(interrupt_state *state)
{
    state->registers.eax = keyboard_get_key_state(state->registers.ebx);
}