#ifndef MICROS_GENERIC_VGA_H
#define MICROS_GENERIC_VGA_H

#include <stdbool.h>
#include "micros_interrupts.h"

#ifdef __cplusplus
extern "C" {
#endif

void micros_generic_vga_set_video_mode(uint16_t mode);
bool micros_generic_vga_is_text_mode();
uint16_t micros_generic_vga_get_current_video_mode();

#ifdef __cplusplus
}
#endif

#endif