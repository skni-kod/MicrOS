#ifndef GENERICVGA_H
#define GENERICVGA_H

#include <stdint.h>
#include "drivers/dal/videocard/videocard.h"

void generic_vga_driver_init();
int16_t generic_vga_set_video_mode(uint16_t mode);
uint8_t generic_vga_is_text_mode();
VideoMode *generic_vga_get_current_video_mode();

#endif