#include "keyboard_calls.h"

void syscall_keyboard_is_key_pressed(interrupt_state *state)
{
    terminal_struct* terminal = find_terminal_for_process(process_manager_get_current_process()->id);
    uint32_t active_terminal_id = terminal_manager_get_active_terminal_id();
    
    if (terminal->terminal_id == active_terminal_id)
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
    terminal_struct* terminal = find_terminal_for_process(process_manager_get_current_process()->id);
    uint32_t active_terminal_id = terminal_manager_get_active_terminal_id();
    
    if (terminal->terminal_id == active_terminal_id)
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