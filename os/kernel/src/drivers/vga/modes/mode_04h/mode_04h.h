#ifndef MODE_04H
#define MODE_04H

#include "stdint.h"

#define mode04h_WIDTH 320
#define mode04h_HEIGHT 200

int8_t mode04h_set_mode();

int8_t mode04h_turn_on_buffer();
int8_t mode04h_turn_off_buffer();
uint8_t mode04h_is_buffer_on();

int8_t mode04h_swap_buffers();

int8_t mode04h_draw_pixel(uint8_t color, uint16_t x, uint16_t y);
int8_t mode04h_draw_line(uint8_t color, uint16_t ax, uint16_t ay, uint16_t bx, uint16_t by);
int8_t mode04h_draw_circle(uint8_t color, uint16_t x, uint16_t y, uint16_t radius);
int8_t mode04h_draw_rectangle(uint8_t color, uint16_t ax, uint16_t ay, uint16_t bx, uint16_t by);
int8_t mode04h_clear_screen();

int8_t mode04h_draw_pixel_buffered(uint8_t color, uint16_t x, uint16_t y);
int8_t mode04h_draw_line_buffered(uint8_t color, uint16_t ax, uint16_t ay, uint16_t bx, uint16_t by);
int8_t mode04h_draw_circle_buffered(uint8_t color, uint16_t x, uint16_t y, uint16_t radius);
int8_t mode04h_draw_rectangle_buffered(uint8_t color, uint16_t ax, uint16_t ay, uint16_t bx, uint16_t by);
int8_t mode04h_clear_screen_buffered();

#endif