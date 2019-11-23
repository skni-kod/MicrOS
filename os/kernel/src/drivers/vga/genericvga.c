#include "genericvga.h"
#include "drivers/dal/videocard/videocard.h"
#include "modes/mode_00h/mode_00h.h"
#include "modes/mode_01h/mode_01h.h"
#include "modes/mode_02h/mode_02h.h"
#include "modes/mode_03h/mode_03h.h"
#include "modes/mode_04h/mode_04h.h"
#include "modes/mode_05h/mode_05h.h"
#include "modes/mode_06h/mode_06h.h"
#include "modes/mode_07h/mode_07h.h"
#include "modes/mode_0dh/mode_0dh.h"
#include "modes/mode_0eh/mode_0eh.h"
#include "modes/mode_0fh/mode_0fh.h"
#include "modes/mode_10h/mode_10h.h"
#include "modes/mode_11h/mode_11h.h"
#include "modes/mode_12h/mode_12h.h"
#include "modes/mode_13h/mode_13h.h"
#include "modes/mode_y/mode_y.h"

video_mode current_video_mode;
uint8_t text_mode;

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

    current_video_mode.colors = 16;
    current_video_mode.height = 25;
    current_video_mode.width = 80;
    current_video_mode.id = 0x03;
    current_video_mode.monochrome = 0;
    current_video_mode.planar = 0;

    text_mode = 1;
}

int16_t generic_vga_set_video_mode(uint16_t mode){
    switch(mode)
    {
        case 0x00:
            video_card_turn_off_buffer();
            if(mode00h_set_mode() != 0x00) return -1;
            current_video_mode.colors = 16;
            current_video_mode.height = 25;
            current_video_mode.width = 40;
            current_video_mode.id = 0x00;
            current_video_mode.monochrome = 1;
            current_video_mode.planar = 0;
            text_mode = 1;
            return 0x00;
        case 0x01:
            video_card_turn_off_buffer();
            if(mode01h_set_mode() != 0x01) return -1;
            current_video_mode.colors = 16;
            current_video_mode.height = 25;
            current_video_mode.width = 40;
            current_video_mode.id = 0x01;
            current_video_mode.monochrome = 0;
            current_video_mode.planar = 0;
            text_mode = 1;
            return 0x01;
        case 0x02:
            video_card_turn_off_buffer();
            if(mode02h_set_mode() != 0x02) return -1;
            current_video_mode.colors = 16;
            current_video_mode.height = 25;
            current_video_mode.width = 80;
            current_video_mode.id = 0x02;
            current_video_mode.monochrome = 1;
            current_video_mode.planar = 0;
            text_mode = 1;
            return 0x02;
        case 0x03:
            video_card_turn_off_buffer();
            if(mode03h_set_mode() != 0x03) return -1;
            current_video_mode.colors = 16;
            current_video_mode.height = 25;
            current_video_mode.width = 80;
            current_video_mode.id = 0x03;
            current_video_mode.monochrome = 0;
            current_video_mode.planar = 0;
            text_mode = 1;
            return 0x03;
        case 0x04:
            video_card_turn_off_buffer();
            if(mode04h_set_mode() != 0x04) return -1;
            current_video_mode.colors = 4;
            current_video_mode.height = 200;
            current_video_mode.width = 320;
            current_video_mode.id = 0x04;
            current_video_mode.monochrome = 0;
            current_video_mode.planar = 0;
            text_mode = 0;
            return 0x04;
        case 0x05:
            video_card_turn_off_buffer();
            if(mode05h_set_mode() != 0x05) return -1;
            current_video_mode.colors = 4;
            current_video_mode.height = 200;
            current_video_mode.width = 320;
            current_video_mode.id = 0x05;
            current_video_mode.monochrome = 0;
            current_video_mode.planar = 0;
            text_mode = 0;
            return 0x05;
        case 0x06:
            video_card_turn_off_buffer();
            if(mode06h_set_mode() != 0x06) return -1;
            current_video_mode.colors = 2;
            current_video_mode.height = 200;
            current_video_mode.width = 640;
            current_video_mode.id = 0x06;
            current_video_mode.monochrome = 1;
            current_video_mode.planar = 0;
            text_mode = 0;
            return 0x06;
        case 0x07:
            video_card_turn_off_buffer();
            if(mode07h_set_mode() != 0x07) return -1;
            current_video_mode.colors = 3;
            current_video_mode.height = 25;
            current_video_mode.width = 80;
            current_video_mode.id = 0x07;
            current_video_mode.monochrome = 1;
            current_video_mode.planar = 0;
            text_mode = 1;
            return 0x07;
        case 0x0D:
            video_card_turn_off_buffer();
            if(mode0dh_set_mode() != 0x0D) return -1;
            current_video_mode.colors = 16;
            current_video_mode.height = 200;
            current_video_mode.width = 320;
            current_video_mode.id = 0x0D;
            current_video_mode.monochrome = 0;
            current_video_mode.planar = 1;
            text_mode = 0;
            return 0x0D;
        case 0x0E:
            video_card_turn_off_buffer();
            if(mode0eh_set_mode() != 0x0E) return -1;
            current_video_mode.colors = 16;
            current_video_mode.height = 200;
            current_video_mode.width = 640;
            current_video_mode.id = 0x0E;
            current_video_mode.monochrome = 0;
            current_video_mode.planar = 1;
            text_mode = 0;
            return 0x0E;
        case 0x0F:
            video_card_turn_off_buffer();
            if(mode0fh_set_mode() != 0x0F) return -1;
            current_video_mode.colors = 4;
            current_video_mode.height = 350;
            current_video_mode.width = 640;
            current_video_mode.id = 0x0F;
            current_video_mode.monochrome = 1;
            current_video_mode.planar = 1;
            text_mode = 0;
            return 0x0F;
        case 0x10:
            video_card_turn_off_buffer();
            if(mode10h_set_mode() != 0x10) return -1;
            current_video_mode.colors = 16;
            current_video_mode.height = 350;
            current_video_mode.width = 640;
            current_video_mode.id = 0x10;
            current_video_mode.monochrome = 0;
            current_video_mode.planar = 1;
            text_mode = 0;
            return 0x10;
        case 0x11:
            video_card_turn_off_buffer();
            if(mode11h_set_mode() != 0x11) return -1;
            current_video_mode.colors = 2;
            current_video_mode.height = 480;
            current_video_mode.width = 640;
            current_video_mode.id = 0x11;
            current_video_mode.monochrome = 1;
            current_video_mode.planar = 1;
            text_mode = 0;
            return 0x11;
        case 0x12:
            video_card_turn_off_buffer();
            if(mode12h_set_mode() != 0x12) return -1;
            current_video_mode.colors = 16;
            current_video_mode.height = 480;
            current_video_mode.width = 640;
            current_video_mode.id = 0x12;
            current_video_mode.monochrome = 0;
            current_video_mode.planar = 1;
            text_mode = 0;
            return 0x12;
        case 0x13:
            video_card_turn_off_buffer();
            if(mode13h_set_mode() != 0x13) return -1;
            current_video_mode.colors = 256;
            current_video_mode.height = 200;
            current_video_mode.width = 320;
            current_video_mode.id = 0x13;
            current_video_mode.monochrome = 0;
            current_video_mode.planar = 0;
            text_mode = 0;
            return 0x13;
        case 0x69:
            video_card_turn_off_buffer();
            if(modey_set_mode() != 0x69) return -1;
            current_video_mode.colors = 256;
            current_video_mode.height = 200;
            current_video_mode.width = 320;
            current_video_mode.id = 0x69;
            current_video_mode.monochrome = 0;
            current_video_mode.planar = 1;
            text_mode = 0;
            return 0x69;
        default: return -1;
    }
}

uint8_t generic_vga_is_text_mode()
{
    return text_mode;
}

video_mode *generic_vga_get_current_video_mode()
{
    return &current_video_mode;
}