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
#include "memory/heap/heap.h"

#include "../../assembly/io.h"

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

    s.print_char = mode03h_print_char;
    s.print_char_color = mode03h_print_char_color;
    s.print_string = mode03h_print_string;
    s.print_string_color = mode03h_print_string_color;
    s.set_char = mode03h_set_char;
    s.get_char = mode03h_get_char;
    s.set_color = mode03h_set_color;
    s.get_color = mode03h_get_color;
    s.set_char_and_color = mode03h_set_char_and_color;
    s.get_char_and_color = mode03h_get_char_and_color;
    s.set_cursor_pos = mode03h_set_cursor_pos;
    s.get_cursor_pos = mode03h_get_cursor_pos;
    s.turn_cursor_on = mode03h_turn_cursor_on;
    s.turn_cursor_off = mode03h_turn_cursor_off;

    s.print_char_external_buffer = generic_vga_print_char_external_buffer;
    s.print_char_color_external_buffer = generic_vga_print_char_color_external_buffer;
    s.print_string_external_buffer = generic_vga_print_string_external_buffer;
    s.print_string_color_external_buffer = generic_vga_print_string_color_external_buffer;
    s.set_char_external_buffer = generic_vga_set_char_external_buffer;
    s.get_char_external_buffer = generic_vga_get_char_external_buffer;
    s.set_color_external_buffer = generic_vga_set_color_external_buffer;
    s.get_color_external_buffer = generic_vga_get_color_external_buffer;
    s.set_char_and_color_external_buffer = generic_vga_set_char_and_color_external_buffer;
    s.get_char_and_color_external_buffer = generic_vga_get_char_and_color_external_buffer;

    s.draw_pixel_external_buffer = generic_vga_draw_pixel_external_buffer;
    s.draw_line_external_buffer = generic_vga_draw_line_external_buffer;
    s.draw_circle_external_buffer = generic_vga_draw_circle_external_buffer;
    s.draw_rectangle_external_buffer = generic_vga_draw_rectangle_external_buffer;
    s.clear_screen_external_buffer = generic_vga_clear_screen_external_buffer;

    s.swap_external_buffer = generic_vga_swap_external_buffer;
    s.create_external_buffer = generic_vga_create_external_buffer;
    s.destroy_external_buffer = generic_vga_destroy_external_buffer;
    
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

int8_t generic_vga_draw_pixel_external_buffer(uint8_t* buffer, uint16_t mode, int8_t color, uint16_t x, uint16_t y){
    switch(mode)
    {
        case 0x00:
            return mode00h_draw_pixel_external_buffer(buffer, mode, color, x, y);
        case 0x01:
            return mode01h_draw_pixel_external_buffer(buffer, mode, color, x, y);
        case 0x02:
            return mode02h_draw_pixel_external_buffer(buffer, mode, color, x, y);
        case 0x03:
            return mode03h_draw_pixel_external_buffer(buffer, mode, color, x, y);
        case 0x04:
            return mode04h_draw_pixel_external_buffer(buffer, mode, color, x, y);
        case 0x05:
            return mode05h_draw_pixel_external_buffer(buffer, mode, color, x, y);
        case 0x06:
            return mode06h_draw_pixel_external_buffer(buffer, mode, color, x, y);
        case 0x07:
            return mode07h_draw_pixel_external_buffer(buffer, mode, color, x, y);
        case 0x0D:
            return mode0dh_draw_pixel_external_buffer(buffer, mode, color, x, y);
        case 0x0E:
            return mode0eh_draw_pixel_external_buffer(buffer, mode, color, x, y);
        case 0x0F:
            return mode0fh_draw_pixel_external_buffer(buffer, mode, color, x, y);
        case 0x10:
            return mode10h_draw_pixel_external_buffer(buffer, mode, color, x, y);
        case 0x11:
            return mode11h_draw_pixel_external_buffer(buffer, mode, color, x, y);
        case 0x12:
            return mode12h_draw_pixel_external_buffer(buffer, mode, color, x, y);
        case 0x13:
            return mode13h_draw_pixel_external_buffer(buffer, mode, color, x, y);
        case 0x69:
            return modey_draw_pixel_external_buffer(buffer, mode, color, x, y);
        default: return -1;
    }
}

int8_t generic_vga_draw_line_external_buffer(uint8_t* buffer, uint16_t mode, uint8_t color, uint16_t ax, uint16_t ay, uint16_t bx, uint16_t by){
    switch(mode)
    {
        case 0x00:
            return mode00h_draw_line_external_buffer(buffer, mode, color, ax, ay, bx, by);
        case 0x01:
            return mode01h_draw_line_external_buffer(buffer, mode, color, ax, ay, bx, by);
        case 0x02:
            return mode02h_draw_line_external_buffer(buffer, mode, color, ax, ay, bx, by);
        case 0x03:
            return mode03h_draw_line_external_buffer(buffer, mode, color, ax, ay, bx, by);
        case 0x04:
            return mode04h_draw_line_external_buffer(buffer, mode, color, ax, ay, bx, by);
        case 0x05:
            return mode05h_draw_line_external_buffer(buffer, mode, color, ax, ay, bx, by);
        case 0x06:
            return mode06h_draw_line_external_buffer(buffer, mode, color, ax, ay, bx, by);
        case 0x07:
            return mode07h_draw_line_external_buffer(buffer, mode, color, ax, ay, bx, by);
        case 0x0D:
            return mode0dh_draw_line_external_buffer(buffer, mode, color, ax, ay, bx, by);
        case 0x0E:
            return mode0eh_draw_line_external_buffer(buffer, mode, color, ax, ay, bx, by);
        case 0x0F:
            return mode0fh_draw_line_external_buffer(buffer, mode, color, ax, ay, bx, by);
        case 0x10:
            return mode10h_draw_line_external_buffer(buffer, mode, color, ax, ay, bx, by);
        case 0x11:
            return mode11h_draw_line_external_buffer(buffer, mode, color, ax, ay, bx, by);
        case 0x12:
            return mode12h_draw_line_external_buffer(buffer, mode, color, ax, ay, bx, by);
        case 0x13:
            return mode13h_draw_line_external_buffer(buffer, mode, color, ax, ay, bx, by);
        case 0x69:
            return modey_draw_line_external_buffer(buffer, mode, color, ax, ay, bx, by);
        default: return -1;
    }
}
int8_t generic_vga_draw_circle_external_buffer(uint8_t* buffer, uint16_t mode, uint8_t color, uint16_t x, uint16_t y, uint16_t radius){
    switch(mode)
    {
        case 0x00:
            return mode00h_draw_circle_external_buffer(buffer, mode, color, x, y, radius);
        case 0x01:
            return mode01h_draw_circle_external_buffer(buffer, mode, color, x, y, radius);
        case 0x02:
            return mode02h_draw_circle_external_buffer(buffer, mode, color, x, y, radius);
        case 0x03:
            return mode03h_draw_circle_external_buffer(buffer, mode, color, x, y, radius);
        case 0x04:
            return mode04h_draw_circle_external_buffer(buffer, mode, color, x, y, radius);
        case 0x05:
            return mode05h_draw_circle_external_buffer(buffer, mode, color, x, y, radius);
        case 0x06:
            return mode06h_draw_circle_external_buffer(buffer, mode, color, x, y, radius);
        case 0x07:
            return mode07h_draw_circle_external_buffer(buffer, mode, color, x, y, radius);
        case 0x0D:
            return mode0dh_draw_circle_external_buffer(buffer, mode, color, x, y, radius);
        case 0x0E:
            return mode0eh_draw_circle_external_buffer(buffer, mode, color, x, y, radius);
        case 0x0F:
            return mode0fh_draw_circle_external_buffer(buffer, mode, color, x, y, radius);
        case 0x10:
            return mode10h_draw_circle_external_buffer(buffer, mode, color, x, y, radius);
        case 0x11:
            return mode11h_draw_circle_external_buffer(buffer, mode, color, x, y, radius);
        case 0x12:
            return mode12h_draw_circle_external_buffer(buffer, mode, color, x, y, radius);
        case 0x13:
            return mode13h_draw_circle_external_buffer(buffer, mode, color, x, y, radius);
        case 0x69:
            return modey_draw_circle_external_buffer(buffer, mode, color, x, y, radius);
        default: return -1;
    }
}
int8_t generic_vga_draw_rectangle_external_buffer(uint8_t* buffer, uint16_t mode, uint8_t color, uint16_t ax, uint16_t ay, uint16_t bx, uint16_t by){
    switch(mode)
    {
        case 0x00:
            return mode00h_draw_rectangle_external_buffer(buffer, mode, color, ax, ay, bx, by);
        case 0x01:
            return mode01h_draw_rectangle_external_buffer(buffer, mode, color, ax, ay, bx, by);
        case 0x02:
            return mode02h_draw_rectangle_external_buffer(buffer, mode, color, ax, ay, bx, by);
        case 0x03:
            return mode03h_draw_rectangle_external_buffer(buffer, mode, color, ax, ay, bx, by);
        case 0x04:
            return mode04h_draw_rectangle_external_buffer(buffer, mode, color, ax, ay, bx, by);
        case 0x05:
            return mode05h_draw_rectangle_external_buffer(buffer, mode, color, ax, ay, bx, by);
        case 0x06:
            return mode06h_draw_rectangle_external_buffer(buffer, mode, color, ax, ay, bx, by);
        case 0x07:
            return mode07h_draw_rectangle_external_buffer(buffer, mode, color, ax, ay, bx, by);
        case 0x0D:
            return mode0dh_draw_rectangle_external_buffer(buffer, mode, color, ax, ay, bx, by);
        case 0x0E:
            return mode0eh_draw_rectangle_external_buffer(buffer, mode, color, ax, ay, bx, by);
        case 0x0F:
            return mode0fh_draw_rectangle_external_buffer(buffer, mode, color, ax, ay, bx, by);
        case 0x10:
            return mode10h_draw_rectangle_external_buffer(buffer, mode, color, ax, ay, bx, by);
        case 0x11:
            return mode11h_draw_rectangle_external_buffer(buffer, mode, color, ax, ay, bx, by);
        case 0x12:
            return mode12h_draw_rectangle_external_buffer(buffer, mode, color, ax, ay, bx, by);
        case 0x13:
            return mode13h_draw_rectangle_external_buffer(buffer, mode, color, ax, ay, bx, by);
        case 0x69:
            return modey_draw_rectangle_external_buffer(buffer, mode, color, ax, ay, bx, by);
        default: return -1;
    }
}
int8_t generic_vga_clear_screen_external_buffer(uint8_t* buffer, uint16_t mode, uint16_t* x, uint16_t* y){
    switch(mode)
    {
        case 0x00:
            return mode00h_clear_screen_external_buffer(buffer, mode, x, y);
        case 0x01:
            return mode01h_clear_screen_external_buffer(buffer, mode, x, y);
        case 0x02:
            return mode02h_clear_screen_external_buffer(buffer, mode, x, y);
        case 0x03:
            return mode03h_clear_screen_external_buffer(buffer, mode, x, y);
        case 0x04:
            return mode04h_clear_screen_external_buffer(buffer, mode, x, y);
        case 0x05:
            return mode05h_clear_screen_external_buffer(buffer, mode, x, y);
        case 0x06: 
            return mode06h_clear_screen_external_buffer(buffer, mode, x, y);
        case 0x07:
            return mode07h_clear_screen_external_buffer(buffer, mode, x, y);
        case 0x0D:
            return mode0dh_clear_screen_external_buffer(buffer, mode, x, y);
        case 0x0E:
            return mode0eh_clear_screen_external_buffer(buffer, mode, x, y);
        case 0x0F:
            return mode0fh_clear_screen_external_buffer(buffer, mode, x, y);
        case 0x10:
            return mode10h_clear_screen_external_buffer(buffer, mode, x, y);
        case 0x11:
            return mode11h_clear_screen_external_buffer(buffer, mode, x, y);
        case 0x12:
            return mode12h_clear_screen_external_buffer(buffer, mode, x, y);
        case 0x13:
            return mode13h_clear_screen_external_buffer(buffer, mode, x, y);
        case 0x69:
            return modey_clear_screen_external_buffer(buffer, mode, x, y);
        default: return -1;
    }
}

int8_t generic_vga_swap_external_buffer(uint8_t* buffer, uint16_t mode){
    switch(mode)
    {
        case 0x00:
            return mode00h_swap_external_buffer(buffer, mode);
        case 0x01:
            return mode01h_swap_external_buffer(buffer, mode);
        case 0x02:
            return mode02h_swap_external_buffer(buffer, mode);
        case 0x03:
            return mode03h_swap_external_buffer(buffer, mode);
        case 0x04:
            return mode04h_swap_external_buffer(buffer, mode);
        case 0x05:
            return mode05h_swap_external_buffer(buffer, mode);
        case 0x06: 
            return mode06h_swap_external_buffer(buffer, mode);
        case 0x07:
            return mode07h_swap_external_buffer(buffer, mode);
        case 0x0D:
            return mode0dh_swap_external_buffer(buffer, mode);
        case 0x0E:
            return mode0eh_swap_external_buffer(buffer, mode);
        case 0x0F:
            return mode0fh_swap_external_buffer(buffer, mode);
        case 0x10:
            return mode10h_swap_external_buffer(buffer, mode);
        case 0x11:
            return mode11h_swap_external_buffer(buffer, mode);
        case 0x12:
            return mode12h_swap_external_buffer(buffer, mode);
        case 0x13:
            return mode13h_swap_external_buffer(buffer, mode);
        case 0x69:
            return modey_swap_external_buffer(buffer, mode);
        default: return -1;
    }
}
uint8_t* generic_vga_create_external_buffer(uint16_t mode)
{
    switch(mode)
    {
         case 0x00:
            return mode00h_create_external_buffer(mode);
        case 0x01:
            return mode01h_create_external_buffer(mode);
        case 0x02:
            return mode02h_create_external_buffer(mode);
        case 0x03:
            return mode03h_create_external_buffer(mode);
        case 0x04:
            return mode04h_create_external_buffer(mode);
        case 0x05:
            return mode05h_create_external_buffer(mode);
        case 0x06: 
            return mode06h_create_external_buffer(mode);
        case 0x07:
            return mode07h_create_external_buffer(mode);
        case 0x0D:
            return mode0dh_create_external_buffer(mode);
        case 0x0E:
            return mode0eh_create_external_buffer(mode);
        case 0x0F:
            return mode0fh_create_external_buffer(mode);
        case 0x10:
            return mode10h_create_external_buffer(mode);
        case 0x11:
            return mode11h_create_external_buffer(mode);
        case 0x12:
            return mode12h_create_external_buffer(mode);
        case 0x13:
            return mode13h_create_external_buffer(mode);
        case 0x69:
            return modey_create_external_buffer(mode);
        default: return NULL;
    }
}

int8_t generic_vga_print_char_external_buffer(uint8_t* buffer, uint16_t mode, uint16_t* x, uint16_t* y, char character)
{
    switch(mode)
    {
        case 0x00:
            return mode00h_print_char_external_buffer(buffer, mode, x, y, character);
        case 0x01:
            return mode01h_print_char_external_buffer(buffer, mode, x, y, character);
        case 0x02:
            return mode02h_print_char_external_buffer(buffer, mode, x, y, character);
        case 0x03:
            return mode03h_print_char_external_buffer(buffer, mode, x, y, character);
        case 0x07:
            return mode07h_print_char_external_buffer(buffer, mode, x, y, character);
        default: 
            return -1;
    }
}
int8_t generic_vga_print_char_color_external_buffer(uint8_t* buffer, uint16_t mode, uint16_t* x, uint16_t* y, char character, uint8_t color)
{
    switch(mode)
    {
        case 0x00:
            return mode00h_print_char_color_external_buffer(buffer, mode, x, y, character, color);
        case 0x01:
            return mode01h_print_char_color_external_buffer(buffer, mode, x, y, character, color);
        case 0x02:
            return mode02h_print_char_color_external_buffer(buffer, mode, x, y, character, color);
        case 0x03:
            return mode03h_print_char_color_external_buffer(buffer, mode, x, y, character, color);
        case 0x07:
            return mode07h_print_char_color_external_buffer(buffer, mode, x, y, character, color);
        default: 
            return -1;
    }
}
int8_t generic_vga_print_string_external_buffer(uint8_t* buffer, uint16_t mode, uint16_t* x, uint16_t* y, const char* string)
{
    switch(mode)
    {
        case 0x00:
            return mode00h_print_string_external_buffer(buffer, mode, x, y, string);
        case 0x01:
            return mode01h_print_string_external_buffer(buffer, mode, x, y, string);
        case 0x02:
            return mode02h_print_string_external_buffer(buffer, mode, x, y, string);
        case 0x03:
            return mode03h_print_string_external_buffer(buffer, mode, x, y, string);
        case 0x07:
            return mode07h_print_string_external_buffer(buffer, mode, x, y, string);
        default: 
            return -1;
    }
}
int8_t generic_vga_print_string_color_external_buffer(uint8_t* buffer, uint16_t mode, uint16_t* x, uint16_t* y, const char* string, uint8_t color)
{
    switch(mode)
    {
        case 0x00:
            return mode00h_print_string_color_external_buffer(buffer, mode, x,  y, string, color);
        case 0x01:
            return mode01h_print_string_color_external_buffer(buffer, mode, x,  y, string, color);
        case 0x02:
            return mode02h_print_string_color_external_buffer(buffer, mode, x,  y, string, color);
        case 0x03:
            return mode03h_print_string_color_external_buffer(buffer, mode, x,  y, string, color);
        case 0x07:
            return mode07h_print_string_color_external_buffer(buffer, mode, x,  y, string, color);
        default: 
            return -1;
    }
}
int8_t generic_vga_set_char_external_buffer(uint8_t* buffer, uint16_t mode, uint16_t x, uint16_t y, char character)
{
    switch(mode)
    {
        case 0x00:
            return mode00h_set_char_external_buffer(buffer, mode, x, y, character);
        case 0x01:
            return mode01h_set_char_external_buffer(buffer, mode, x, y, character);
        case 0x02:
            return mode02h_set_char_external_buffer(buffer, mode, x, y, character);
        case 0x03:
            return mode03h_set_char_external_buffer(buffer, mode, x, y, character);
        case 0x07:
            return mode07h_set_char_external_buffer(buffer, mode, x, y, character);
        default: 
            return -1;
    }
}
int8_t generic_vga_get_char_external_buffer(uint8_t* buffer, uint16_t mode, uint16_t x, uint16_t y, char* character)
{
    switch(mode)
    {
        case 0x00:
            return mode00h_get_char_external_buffer(buffer,  mode, x, y, character);
        case 0x01:
            return mode01h_get_char_external_buffer(buffer,  mode, x, y, character);
        case 0x02:
            return mode02h_get_char_external_buffer(buffer,  mode, x, y, character);
        case 0x03:
            return mode03h_get_char_external_buffer(buffer,  mode, x, y, character);
        case 0x07:
            return mode07h_get_char_external_buffer(buffer,  mode, x, y, character);
        default: 
            return -1;
    }
}
int8_t generic_vga_set_color_external_buffer(uint8_t* buffer, uint16_t mode, uint16_t x, uint16_t y, uint8_t color)
{
    switch(mode)
    {
        case 0x00:
            return mode00h_set_color_external_buffer(buffer, mode, x, y, color);
        case 0x01:
            return mode01h_set_color_external_buffer(buffer, mode, x, y, color);
        case 0x02:
            return mode02h_set_color_external_buffer(buffer, mode, x, y, color);
        case 0x03:
            return mode03h_set_color_external_buffer(buffer, mode, x, y, color);
        case 0x07:
            return mode07h_set_color_external_buffer(buffer, mode, x, y, color);
        default: 
            return -1;
    }
}
int8_t generic_vga_get_color_external_buffer(uint8_t* buffer, uint16_t mode, uint16_t x, uint16_t y, uint8_t* color)
{
    switch(mode)
    {
        case 0x00:
            return mode00h_get_color_external_buffer(buffer, mode,  x, y, color);
        case 0x01:
            return mode01h_get_color_external_buffer(buffer, mode,  x, y, color);
        case 0x02:
            return mode02h_get_color_external_buffer(buffer, mode,  x, y, color);
        case 0x03:
            return mode03h_get_color_external_buffer(buffer, mode,  x, y, color);
        case 0x07:
            return mode07h_get_color_external_buffer(buffer, mode,  x, y, color);
        default: 
            return -1;
    }
}
int8_t generic_vga_set_char_and_color_external_buffer(uint8_t* buffer, uint16_t mode, uint16_t x, uint16_t y, char character, uint8_t color)
{
    switch(mode)
    {
        case 0x00:
            return mode00h_set_char_and_color_external_buffer(buffer, mode, x, y, character,  color);
        case 0x01:
            return mode01h_set_char_and_color_external_buffer(buffer, mode, x, y, character,  color);
        case 0x02:
            return mode02h_set_char_and_color_external_buffer(buffer, mode, x, y, character,  color);
        case 0x03:
            return mode03h_set_char_and_color_external_buffer(buffer, mode, x, y, character,  color);
        case 0x07:
            return mode07h_set_char_and_color_external_buffer(buffer, mode, x, y, character,  color);
        default: 
            return -1;
    }
}
int8_t generic_vga_get_char_and_color_external_buffer(uint8_t* buffer, uint16_t mode, uint16_t x, uint16_t y, char* character, uint8_t* color)
{
    switch(mode)
    {
        case 0x00:
            return mode00h_get_char_and_color_external_buffer(buffer, mode, x, y, character, color);
        case 0x01:
            return mode01h_get_char_and_color_external_buffer(buffer, mode, x, y, character, color);
        case 0x02:
            return mode02h_get_char_and_color_external_buffer(buffer, mode, x, y, character, color);
        case 0x03:
            return mode03h_get_char_and_color_external_buffer(buffer, mode, x, y, character, color);
        case 0x07:
            return mode07h_get_char_and_color_external_buffer(buffer, mode, x, y, character, color);
        default: 
            return -1;
    }
}

void generic_vga_destroy_external_buffer(uint8_t* buffer)
{
    heap_kernel_dealloc(buffer);
}

uint8_t generic_vga_is_vretrace()
{
    return io_in_byte(INPUT_STATUS_1) & VRETRACE;
}