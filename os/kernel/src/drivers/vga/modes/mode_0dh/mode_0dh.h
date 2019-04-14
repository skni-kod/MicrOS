#ifndef MODE_0DH
#define MODE_0DH

#include "stdint.h"

#define MODE0DH_WIDTH 320
#define MODE0DH_HEIGHT 200

int8_t mode0dh_set_mode();

int8_t mode0dh_turn_on_buffer();
int8_t mode0dh_turn_off_buffer();
uint8_t mode0dh_is_buffer_on();

int8_t mode0dh_swap_buffers();

int8_t mode0dh_draw_pixel(uint8_t color, uint16_t x, uint16_t y);
int8_t mode0dh_draw_line(uint8_t color, uint16_t ax, uint16_t ay, uint16_t bx, uint16_t by);
int8_t mode0dh_draw_circle(uint8_t color, uint16_t x, uint16_t y, uint16_t radius);
int8_t mode0dh_draw_rectangle(uint8_t color, uint16_t ax, uint16_t ay, uint16_t bx, uint16_t by);
int8_t mode0dh_clear_screen();

int8_t mode0dh_draw_pixel_buffered(uint8_t color, uint16_t x, uint16_t y);
int8_t mode0dh_draw_line_buffered(uint8_t color, uint16_t ax, uint16_t ay, uint16_t bx, uint16_t by);
int8_t mode0dh_draw_circle_buffered(uint8_t color, uint16_t x, uint16_t y, uint16_t radius);
int8_t mode0dh_draw_rectangle_buffered(uint8_t color, uint16_t ax, uint16_t ay, uint16_t bx, uint16_t by);
int8_t mode0dh_clear_screen_buffered();

#endif