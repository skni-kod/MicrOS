#ifndef MICROS_CONSOLE_H
#define MICROS_CONSOLE_H

#include <stdint.h>
#include "micros_interrupts.h"

typedef struct micros_console_position
{
    uint16_t x;
    uint16_t y;
} micros_console_position;

typedef enum micros_console_color
{
    micros_console_color_black = 0x00,
    micros_console_color_blue = 0x01,
    micros_console_color_green = 0x02,
    micros_console_color_cyan = 0x03,
    micros_console_color_red = 0x04,
    micros_console_color_magenta = 0x05,
    micros_console_color_brown = 0x06,
    micros_console_color_light_gray = 0x07,
    micros_console_color_dark_gray = 0x08,
    micros_console_color_light_blue = 0x09,
    micros_console_color_light_green = 0x0A,
    micros_console_color_light_cyan = 0x0B,
    micros_console_color_light_red = 0x0C,
    micros_console_color_light_magenta = 0x0D,
    micros_console_color_yellow = 0x0E,
    micros_console_color_white = 0x0F
} micros_console_color;

void micros_console_print_char(char c);
void micros_console_print_string(const char *str);
char micros_console_get_char_at_position(micros_console_position position);
void micros_console_set_foreground_color(micros_console_color color);
void micros_console_set_background_color(micros_console_color color);
void micros_console_set_cursor_position(micros_console_position position);
void micros_console_get_cursor_position(micros_console_position *micros_console_position);
void micros_console_clear();

#endif