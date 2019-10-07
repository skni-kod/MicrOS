#ifndef MODE_02H
#define MODE_02H

#include "stdint.h"

#define MODE02H_WIDTH 80
#define MODE02H_HEIGHT 25

// TODO: implement functions

int8_t mode02h_set_mode();

int8_t mode02h_turn_on_buffer();
int8_t mode02h_turn_off_buffer();
uint8_t mode02h_is_buffer_on();

int8_t mode02h_swap_buffers();

int8_t mode02h_draw_pixel(uint8_t color, uint16_t x, uint16_t y);
int8_t mode02h_draw_line(uint8_t color, uint16_t ax, uint16_t ay, uint16_t bx, uint16_t by);
int8_t mode02h_draw_circle(uint8_t color, uint16_t x, uint16_t y, uint16_t radius);
int8_t mode02h_draw_rectangle(uint8_t color, uint16_t ax, uint16_t ay, uint16_t bx, uint16_t by);
int8_t mode02h_clear_screen();

int8_t mode02h_draw_pixel_buffered(uint8_t color, uint16_t x, uint16_t y);
int8_t mode02h_draw_line_buffered(uint8_t color, uint16_t ax, uint16_t ay, uint16_t bx, uint16_t by);
int8_t mode02h_draw_circle_buffered(uint8_t color, uint16_t x, uint16_t y, uint16_t radius);
int8_t mode02h_draw_rectangle_buffered(uint8_t color, uint16_t ax, uint16_t ay, uint16_t bx, uint16_t by);
int8_t mode02h_clear_screen_buffered();

#endif