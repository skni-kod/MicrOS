#ifndef VGA_CALLS_H
#define VGA_CALLS_H

#include <stdbool.h>
#include "cpu/idt/state/interrupt_state.h"
#include "drivers/vga/vga.h"

void syscall_vga_print_char(interrupt_state *state);
void syscall_vga_print_string(interrupt_state *state);
void syscall_vga_get_char_at_position(interrupt_state *state);
void syscall_vga_set_cursor_position(interrupt_state *state);
void syscall_vga_get_cursor_position(interrupt_state *state);
void syscall_vga_clear();
void syscall_vga_set_cursor_visibility(interrupt_state *state);

#endif