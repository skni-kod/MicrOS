#include "videocard_calls.h"
#include "../../../drivers/dal/videocard/videocard.h"

void syscall_videocard_find_video_mode(interrupt_state* state)
{
    video_mode_descriptor_k* vmd_ptr = (video_mode_descriptor_k*)state->registers.ebx;
    video_mode* mode_ptr = video_card_find_mode(vmd_ptr->width, vmd_ptr->height, vmd_ptr->colors, vmd_ptr->text, vmd_ptr->planar, vmd_ptr->grayscale);
    state->registers.eax = mode_ptr == NULL ? -1 : 0;
    if(mode_ptr != NULL)
    {
        vmd_ptr->mode_number = mode_ptr->id;
        vmd_ptr->width = mode_ptr->width;
        vmd_ptr->height = mode_ptr->height;
        vmd_ptr->colors = mode_ptr->colors;
        vmd_ptr->text = mode_ptr->text;
        vmd_ptr->planar = mode_ptr->planar;
        vmd_ptr->grayscale = mode_ptr->monochrome;
    }
}

void syscall_videocard_set_video_mode(interrupt_state *state)
{
    state->registers.eax = video_card_set_video_mode(state->registers.ebx);
}

void syscall_videocard_is_text_mode(interrupt_state *state)
{
    state->registers.eax = video_card_is_text_mode();
}

void syscall_videocard_get_current_video_mode(interrupt_state *state)
{
    state->registers.eax = video_card_get_current_video_mode()->id;
}

void syscall_videocard_is_vretrace(interrupt_state* state)
{
    state->registers.eax = generic_vga_is_vretrace();
}