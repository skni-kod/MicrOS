#ifndef GENERICVGA_H
#define GENERICVGA_H

#include <stdint.h>
#include "drivers/dal/videocard/videocard.h"

#define INPUT_STATUS_1 0x03da
#define VRETRACE 0x08

void generic_vga_driver_init();
int16_t generic_vga_set_video_mode(uint16_t mode);
uint8_t generic_vga_is_text_mode();
video_mode *generic_vga_get_current_video_mode();

int8_t generic_vga_draw_pixel_external_buffer(uint8_t* buffer, uint16_t mode, int8_t color, uint16_t x, uint16_t y);
int8_t generic_vga_draw_line_external_buffer(uint8_t* buffer, uint16_t mode, uint8_t color, uint16_t ax, uint16_t ay, uint16_t bx, uint16_t by);
int8_t generic_vga_draw_circle_external_buffer(uint8_t* buffer, uint16_t mode, uint8_t color, uint16_t x, uint16_t y, uint16_t radius);
int8_t generic_vga_draw_rectangle_external_buffer(uint8_t* buffer, uint16_t mode, uint8_t color, uint16_t ax, uint16_t ay, uint16_t bx, uint16_t by);
int8_t generic_vga_clear_screen_external_buffer(uint8_t* buffer, uint16_t mode, uint16_t* x, uint16_t* y);

int8_t generic_vga_print_char_external_buffer(uint8_t* buffer, uint16_t mode, uint16_t* x, uint16_t* y, char character);
int8_t generic_vga_print_char_color_external_buffer(uint8_t* buffer, uint16_t mode, uint16_t* x, uint16_t* y, char character, uint8_t color);
int8_t generic_vga_print_string_external_buffer(uint8_t* buffer, uint16_t mode, uint16_t* x, uint16_t* y, const char* string);
int8_t generic_vga_print_string_color_external_buffer(uint8_t* buffer, uint16_t mode, uint16_t* x, uint16_t* y, const char* string, uint8_t color);
int8_t generic_vga_set_char_external_buffer(uint8_t* buffer, uint16_t mode, uint16_t x, uint16_t y, char character);
int8_t generic_vga_get_char_external_buffer(uint8_t* buffer, uint16_t mode, uint16_t x, uint16_t y, char* character);
int8_t generic_vga_set_color_external_buffer(uint8_t* buffer, uint16_t mode, uint16_t x, uint16_t y, uint8_t color);
int8_t generic_vga_get_color_external_buffer(uint8_t* buffer, uint16_t mode, uint16_t x, uint16_t y, uint8_t* color);
int8_t generic_vga_set_char_and_color_external_buffer(uint8_t* buffer, uint16_t mode, uint16_t x, uint16_t y, char character, uint8_t color);
int8_t generic_vga_get_char_and_color_external_buffer(uint8_t* buffer, uint16_t mode, uint16_t x, uint16_t y, char* character, uint8_t* color);

int8_t generic_vga_swap_external_buffer(uint8_t* buffer, uint16_t mode);
uint8_t* generic_vga_create_external_buffer(uint16_t mode);
void generic_vga_destroy_external_buffer(uint8_t* buffer);

uint8_t generic_vga_is_vretrace();

#endif