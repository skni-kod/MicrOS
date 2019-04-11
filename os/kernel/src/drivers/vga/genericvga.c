#include "genericvga.h"
#include "drivers/dal/videocard/videocard.h"
#include "modes/mode_03h/mode_03h.h"
#include "modes/mode_04h/mode_04h.h"
#include "modes/mode_05h/mode_05h.h"
#include "modes/mode_06h/mode_06h.h"
#include "modes/mode_0Dh/mode_0Dh.h"
#include "modes/mode_0Eh/mode_0Eh.h"
#include "modes/mode_0Fh/mode_0Fh.h"
#include "modes/mode_10h/mode_10h.h"
#include "modes/mode_11h/mode_11h.h"
#include "modes/mode_12h/mode_12h.h"
#include "modes/mode_13h/mode_13h.h"
#include "modes/mode_y/mode_y.h"

video_mode currentvideo_mode;
uint8_t textMode;

void generic_vga_driver_init()
{
    driver_init_struct s;
    s.set_video_mode = generic_vga_set_video_mode;
    s.is_text_mode = generic_vga_is_text_mode;
    s.get_current_video_mode = generic_vga_get_current_video_mode;
    s.turn_on_buffer = mode03h_turn_on_buffer;
    s.turn_off_buffer = mode03h_turn_off_buffer;
    s.is_buffer_on = mode03h_is_buffer_on;
    s.swap_buffers = mode03h_swap_buffers;
    s.draw_pixel = mode03h_draw_pixel;
    s.draw_line = mode03h_draw_line;
    s.draw_circle = mode03h_draw_circle;
    s.draw_rectangle = mode03h_draw_rectangle;
    s.clear_screen = mode03h_clear_screen;
    video_card_init_with_driver(&s);

    currentvideo_mode.colors = 16;
    currentvideo_mode.height = 25;
    currentvideo_mode.width = 80;
    currentvideo_mode.id = 0x03;
    currentvideo_mode.monochrome = 0;
    currentvideo_mode.planar = 0;

    textMode = 1;
}

int16_t generic_vga_set_video_mode(uint16_t mode){
    switch(mode)
    {
        case 0x03:
            video_card_turn_off_buffer();
            if(mode03h_set_mode() != 0x03) return -1;
            currentvideo_mode.colors = 16;
            currentvideo_mode.height = 25;
            currentvideo_mode.width = 80;
            currentvideo_mode.id = 0x03;
            currentvideo_mode.monochrome = 0;
            currentvideo_mode.planar = 0;
            textMode = 1;
            return 0x03;
        case 0x04:
            video_card_turn_off_buffer();
            if(mode04h_set_mode() != 0x04) return -1;
            currentvideo_mode.colors = 4;
            currentvideo_mode.height = 200;
            currentvideo_mode.width = 320;
            currentvideo_mode.id = 0x04;
            currentvideo_mode.monochrome = 0;
            currentvideo_mode.planar = 0;
            textMode = 0;
            return 0x04;
        case 0x05:
            video_card_turn_off_buffer();
            if(mode05h_set_mode() != 0x05) return -1;
            currentvideo_mode.colors = 4;
            currentvideo_mode.height = 200;
            currentvideo_mode.width = 320;
            currentvideo_mode.id = 0x05;
            currentvideo_mode.monochrome = 0;
            currentvideo_mode.planar = 0;
            textMode = 0;
            return 0x05;
        case 0x06:
            video_card_turn_off_buffer();
            if(mode06h_set_mode() != 0x06) return -1;
            currentvideo_mode.colors = 2;
            currentvideo_mode.height = 200;
            currentvideo_mode.width = 640;
            currentvideo_mode.id = 0x06;
            currentvideo_mode.monochrome = 1;
            currentvideo_mode.planar = 0;
            textMode = 0;
            return 0x06;
        case 0x0D:
            video_card_turn_off_buffer();
            if(mode0Dh_set_mode() != 0x0D) return -1;
            currentvideo_mode.colors = 16;
            currentvideo_mode.height = 200;
            currentvideo_mode.width = 320;
            currentvideo_mode.id = 0x0D;
            currentvideo_mode.monochrome = 0;
            currentvideo_mode.planar = 1;
            textMode = 0;
            return 0x0D;
        case 0x0E:
            video_card_turn_off_buffer();
            if(mode0Eh_set_mode() != 0x0E) return -1;
            currentvideo_mode.colors = 16;
            currentvideo_mode.height = 200;
            currentvideo_mode.width = 640;
            currentvideo_mode.id = 0x0E;
            currentvideo_mode.monochrome = 0;
            currentvideo_mode.planar = 1;
            textMode = 0;
            return 0x0E;
        case 0x0F:
            video_card_turn_off_buffer();
            if(mode0Fh_set_mode() != 0x0F) return -1;
            currentvideo_mode.colors = 4;
            currentvideo_mode.height = 350;
            currentvideo_mode.width = 640;
            currentvideo_mode.id = 0x0F;
            currentvideo_mode.monochrome = 1;
            currentvideo_mode.planar = 1;
            textMode = 0;
            return 0x0F;
        case 0x10:
            video_card_turn_off_buffer();
            if(mode10h_set_mode() != 0x10) return -1;
            currentvideo_mode.colors = 16;
            currentvideo_mode.height = 350;
            currentvideo_mode.width = 640;
            currentvideo_mode.id = 0x10;
            currentvideo_mode.monochrome = 0;
            currentvideo_mode.planar = 1;
            textMode = 0;
            return 0x10;
        case 0x11:
            video_card_turn_off_buffer();
            if(mode11h_set_mode() != 0x11) return -1;
            currentvideo_mode.colors = 2;
            currentvideo_mode.height = 480;
            currentvideo_mode.width = 640;
            currentvideo_mode.id = 0x11;
            currentvideo_mode.monochrome = 1;
            currentvideo_mode.planar = 1;
            textMode = 0;
            return 0x11;
        case 0x12:
            video_card_turn_off_buffer();
            if(mode12h_set_mode() != 0x12) return -1;
            currentvideo_mode.colors = 16;
            currentvideo_mode.height = 480;
            currentvideo_mode.width = 640;
            currentvideo_mode.id = 0x12;
            currentvideo_mode.monochrome = 0;
            currentvideo_mode.planar = 1;
            textMode = 0;
            return 0x12;
        case 0x13:
            video_card_turn_off_buffer();
            if(mode13h_set_mode() != 0x13) return -1;
            currentvideo_mode.colors = 256;
            currentvideo_mode.height = 200;
            currentvideo_mode.width = 320;
            currentvideo_mode.id = 0x13;
            currentvideo_mode.monochrome = 0;
            currentvideo_mode.planar = 0;
            textMode = 0;
            return 0x13;
        case 0x69:
            video_card_turn_off_buffer();
            if(modey_set_mode() != 0x69) return -1;
            currentvideo_mode.colors = 256;
            currentvideo_mode.height = 200;
            currentvideo_mode.width = 320;
            currentvideo_mode.id = 0x69;
            currentvideo_mode.monochrome = 0;
            currentvideo_mode.planar = 1;
            textMode = 0;
            return 0x69;
        default: return -1;
    }
}

uint8_t generic_vga_is_text_mode()
{
    return textMode;
}

const video_mode *generic_vga_get_current_video_mode()
{
    return &currentvideo_mode;
}