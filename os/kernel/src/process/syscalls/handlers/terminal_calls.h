#ifndef TERMINAL_CALLS_H
#define TERMINAL_CALLS_H

#include <stdbool.h>
#include "cpu/idt/state/interrupt_state.h"
#include "terminal/terminal_manager.h"
#include "process/manager/process_manager.h"

void syscall_terminal_print_char(interrupt_state *state);
void syscall_terminal_print_string(interrupt_state *state);
void syscall_terminal_get_char_at_position(interrupt_state *state);
void syscall_terminal_set_cursor_position(interrupt_state *state);
void syscall_terminal_get_cursor_position(interrupt_state *state);
void syscall_terminal_clear();
void syscall_terminal_set_cursor_visibility(interrupt_state *state);

void syscall_terminal_set_video_mode(interrupt_state *state);
void syscall_terminal_copy_from_buffer(interrupt_state *state);

#endif