#include "vga_calls.h"

void syscall_print_line_call(interrupt_state *state)
{
    vga_printstring(state->ebx);
}