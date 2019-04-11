#ifndef MODE_0DH
#define MODE_0DH

#include "stdint.h"

#define mode0Dh_WIDTH 320
#define mode0Dh_HEIGHT 200

int8_t mode0Dh_set_mode();

int8_t mode0Dh_turn_on_buffer();
int8_t mode0Dh_turn_off_buffer();
uint8_t mode0Dh_is_buffer_on();

int8_t mode0Dh_swap_buffers();

int8_t mode0Dh_draw_pixel(uint8_t color, uint16_t x, uint16_t y);
int8_t mode0Dh_draw_line(uint8_t color, uint16_t ax, uint16_t ay, uint16_t bx, uint16_t by);
int8_t mode0Dh_draw_circle(uint8_t color, uint16_t x, uint16_t y, uint16_t radius);
int8_t mode0Dh_draw_rectangle(uint8_t color, uint16_t ax, uint16_t ay, uint16_t bx, uint16_t by);
int8_t mode0Dh_clear_screen();

int8_t mode0Dh_draw_pixel_buffered(uint8_t color, uint16_t x, uint16_t y);
int8_t mode0Dh_draw_line_buffered(uint8_t color, uint16_t ax, uint16_t ay, uint16_t bx, uint16_t by);
int8_t mode0Dh_draw_circle_buffered(uint8_t color, uint16_t x, uint16_t y, uint16_t radius);
int8_t mode0Dh_draw_rectangle_buffered(uint8_t color, uint16_t ax, uint16_t ay, uint16_t bx, uint16_t by);
int8_t mode0Dh_clear_screen_buffered();

#endif