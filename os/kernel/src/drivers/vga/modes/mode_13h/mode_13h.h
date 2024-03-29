#ifndef MODE_13H
#define MODE_13H

#include "stdint.h"

#define MODE13H_WIDTH 320
#define MODE13H_HEIGHT 200

int8_t mode13h_set_mode();

int8_t mode13h_turn_on_buffer();
int8_t mode13h_turn_off_buffer();
uint8_t mode13h_is_buffer_on();

int8_t mode13h_swap_buffers();

int8_t mode13h_draw_pixel(uint8_t color, uint16_t x, uint16_t y);
int8_t mode13h_draw_line(uint8_t color, uint16_t ax, uint16_t ay, uint16_t bx, uint16_t by);
int8_t mode13h_draw_circle(uint8_t color, uint16_t x, uint16_t y, uint16_t radius);
int8_t mode13h_draw_rectangle(uint8_t color, uint16_t ax, uint16_t ay, uint16_t bx, uint16_t by);
int8_t mode13h_clear_screen();

int8_t mode13h_draw_pixel_buffered(uint8_t color, uint16_t x, uint16_t y);
int8_t mode13h_draw_line_buffered(uint8_t color, uint16_t ax, uint16_t ay, uint16_t bx, uint16_t by);
int8_t mode13h_draw_circle_buffered(uint8_t color, uint16_t x, uint16_t y, uint16_t radius);
int8_t mode13h_draw_rectangle_buffered(uint8_t color, uint16_t ax, uint16_t ay, uint16_t bx, uint16_t by);
int8_t mode13h_clear_screen_buffered();

int8_t mode13h_draw_pixel_external_buffer(uint8_t* buffer, uint16_t mode, int8_t color, uint16_t x, uint16_t y);
int8_t mode13h_draw_line_external_buffer(uint8_t* buffer, uint16_t mode, uint8_t color, uint16_t ax, uint16_t ay, uint16_t bx, uint16_t by);
int8_t mode13h_draw_circle_external_buffer(uint8_t* buffer, uint16_t mode, uint8_t color, uint16_t x, uint16_t y, uint16_t radius);
int8_t mode13h_draw_rectangle_external_buffer(uint8_t* buffer, uint16_t mode, uint8_t color, uint16_t ax, uint16_t ay, uint16_t bx, uint16_t by);
int8_t mode13h_clear_screen_external_buffer(uint8_t* buffer, uint16_t mode, uint16_t* x, uint16_t* y);

int8_t mode13h_swap_external_buffer(uint8_t* buffer, uint16_t mode);
uint8_t* mode13h_create_external_buffer(uint16_t mode);

int8_t mode13h_print_char(char character);
int8_t mode13h_print_char_color(char character, uint8_t color);
int8_t mode13h_print_string(const char* string);
int8_t mode13h_print_string_color(const char* string, uint8_t color);
int8_t mode13h_set_char(uint16_t x, uint16_t y, char character);
int8_t mode13h_get_char(uint16_t x, uint16_t y, char* character);
int8_t mode13h_set_color(uint16_t x, uint16_t y, uint8_t color);
int8_t mode13h_get_color(uint16_t x, uint16_t y, uint8_t* color);
int8_t mode13h_set_char_and_color(uint16_t x, uint16_t y, char character, uint8_t color);
int8_t mode13h_get_char_and_color(uint16_t x, uint16_t y, char* character, uint8_t* color);
int8_t mode13h_set_cursor_pos(uint16_t x, uint16_t y);
int8_t mode13h_get_cursor_pos(uint16_t* x, uint16_t* y);
int8_t mode13h_turn_cursor_on();
int8_t mode13h_turn_cursor_off();

#endif