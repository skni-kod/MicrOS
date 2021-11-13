#include "generic_vga.h"
#include "../../../drivers/dal/videocard/videocard.h"

void syscall_generic_vga_set_video_mode(interrupt_state *state)
{
    state->registers.eax = generic_vga_set_video_mode(state->registers.ebx);
}

void syscall_generic_vga_is_text_mode(interrupt_state *state)
{
    state->registers.eax = video_card_is_text_mode();
}

void syscall_generic_vga_get_current_video_mode(interrupt_state *state)
{
    state->registers.eax = video_card_get_current_video_mode()->id;
}

void syscall_generic_vga_is_vretrace(interrupt_state* state)
{
    state->registers.eax = generic_vga_is_vretrace();
}