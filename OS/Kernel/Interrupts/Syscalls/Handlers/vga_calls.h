#ifndef VGA_CALLS_H
#define VGA_CALLS_H

#include "../../IDT/interrupt_state.h"
#include "../../../Drivers/VGA/vga.h"

void syscall_print_line_call(interrupt_state *state);

#endif