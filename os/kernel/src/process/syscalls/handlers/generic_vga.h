#ifndef GENERIC_VGA_H
#define GENERIC_VGA_H

#include "drivers/vga/genericvga.h"
#include "cpu/idt/state/interrupt_state.h"

void syscall_generic_vga_set_video_mode(interrupt_state *state);
void syscall_generic_vga_is_text_mode(interrupt_state *state);
void syscall_generic_vga_get_current_video_mode(interrupt_state *state);

//TODO NO DAL
void syscall_generic_vga_is_vretrace(interrupt_state* state);

#endif