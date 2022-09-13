#ifndef MODE_12H
#define MODE_12H

#include "stdint.h"

#define MODE12H_WIDTH 640
#define MODE12H_HEIGHT 480

int8_t mode12h_set_mode();

int8_t mode12h_turn_on_buffer();
int8_t mode12h_turn_off_buffer();
uint8_t mode12h_is_buffer_on();

int8_t mode12h_swap_buffers();

int8_t mode12h_draw_pixel(uint8_t color, uint16_t x, uint16_t y);
int8_t mode12h_draw_line(uint8_t color, uint16_t ax, uint16_t ay, uint16_t bx, uint16_t by);
int8_t mode12h_draw_circle(uint8_t color, uint16_t x, uint16_t y, uint16_t radius);
int8_t mode12h_draw_rectangle(uint8_t color, uint16_t ax, uint16_t ay, uint16_t bx, uint16_t by);
int8_t mode12h_clear_screen();

int8_t mode12h_draw_pixel_buffered(uint8_t color, uint16_t x, uint16_t y);
int8_t mode12h_draw_line_buffered(uint8_t color, uint16_t ax, uint16_t ay, uint16_t bx, uint16_t by);
int8_t mode12h_draw_circle_buffered(uint8_t color, uint16_t x, uint16_t y, uint16_t radius);
int8_t mode12h_draw_rectangle_buffered(uint8_t color, uint16_t ax, uint16_t ay, uint16_t bx, uint16_t by);
int8_t mode12h_clear_screen_buffered();

int8_t mode12h_draw_pixel_external_buffer(uint8_t* buffer, uint16_t mode, int8_t color, uint16_t x, uint16_t y);
int8_t mode12h_draw_line_external_buffer(uint8_t* buffer, uint16_t mode, uint8_t color, uint16_t ax, uint16_t ay, uint16_t bx, uint16_t by);
int8_t mode12h_draw_circle_external_buffer(uint8_t* buffer, uint16_t mode, uint8_t color, uint16_t x, uint16_t y, uint16_t radius);
int8_t mode12h_draw_rectangle_external_buffer(uint8_t* buffer, uint16_t mode, uint8_t color, uint16_t ax, uint16_t ay, uint16_t bx, uint16_t by);
int8_t mode12h_clear_screen_external_buffer(uint8_t* buffer, uint16_t mode, uint16_t* x, uint16_t* y);

int8_t mode12h_swap_external_buffer(uint8_t* buffer, uint16_t mode);
uint8_t* mode12h_create_external_buffer(uint16_t mode);

#endif