#ifndef VIDEOCARD_CALLS_H
#define VIDEOCARD_CALLS_H

#include "../cpu/idt/state/interrupt_state.h"
#include <stddef.h>

typedef struct _video_mode_descriptor_k
{
    uint16_t mode_number;
    uint32_t width;
    uint32_t height;
    uint64_t colors;
    uint8_t text;
    uint8_t grayscale;
    uint8_t planar;
} video_mode_descriptor_k;

void syscall_videocard_find_video_mode(interrupt_state* state);
void syscall_videocard_set_video_mode(interrupt_state *state);
void syscall_videocard_is_text_mode(interrupt_state *state);
void syscall_videocard_get_current_video_mode(interrupt_state *state);

//TODO NO DAL
void syscall_videocard_is_vretrace(interrupt_state* state);

#endif