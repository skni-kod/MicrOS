#ifndef MODE_0EH
#define MODE_0EH

#include "stdint.h"

#define mode0Eh_WIDTH 640
#define mode0Eh_HEIGHT 200

int8_t mode0Eh_set_mode();

int8_t mode0Eh_turn_on_buffer();
int8_t mode0Eh_turn_off_buffer();
uint8_t mode0Eh_is_buffer_on();

int8_t mode0Eh_swap_buffers();

int8_t mode0Eh_draw_pixel(uint8_t color, uint16_t x, uint16_t y);
int8_t mode0Eh_draw_line(uint8_t color, uint16_t ax, uint16_t ay, uint16_t bx, uint16_t by);
int8_t mode0Eh_draw_circle(uint8_t color, uint16_t x, uint16_t y, uint16_t radius);
int8_t mode0Eh_draw_rectangle(uint8_t color, uint16_t ax, uint16_t ay, uint16_t bx, uint16_t by);
int8_t mode0Eh_clear_screen();

int8_t mode0Eh_draw_pixel_buffered(uint8_t color, uint16_t x, uint16_t y);
int8_t mode0Eh_draw_line_buffered(uint8_t color, uint16_t ax, uint16_t ay, uint16_t bx, uint16_t by);
int8_t mode0Eh_draw_circle_buffered(uint8_t color, uint16_t x, uint16_t y, uint16_t radius);
int8_t mode0Eh_draw_rectangle_buffered(uint8_t color, uint16_t ax, uint16_t ay, uint16_t bx, uint16_t by);
int8_t mode0Eh_clear_screen_buffered();

#endif