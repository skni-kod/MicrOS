#ifndef VGA_CALLS_H
#define VGA_CALLS_H

#include <stdbool.h>
#include "../../IDT/interrupt_state.h"
#include "../../../Drivers/VGA/vga.h"

void syscall_vga_print_char(interrupt_state *state);
void syscall_vga_print_string(interrupt_state *state);
char syscall_vga_get_char_at_position(interrupt_state *state);
void syscall_vga_set_cursor_position(interrupt_state *state);
void syscall_vga_get_cursor_position(interrupt_state *state);
void syscall_vga_clear(interrupt_state *state);
void syscall_vga_set_cursor_visibility(interrupt_state *state);

#endif