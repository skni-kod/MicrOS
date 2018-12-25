#include "micros_console.h"

int foreground_color = 7;
int background_color = 0;

void micros_console_print_char(char c)
{
    micros_interrupt_3a(0x10, (uint32_t)c, (uint32_t)foreground_color, (uint32_t)background_color);
}

void micros_console_print_string(const char *str)
{
    micros_interrupt_3a(0x11, (uint32_t)str, (uint32_t)foreground_color, (uint32_t)background_color);
}

char micros_console_get_char_at_position(micros_console_position position)
{
    return (char)micros_interrupt_2a(0x12, position.x, position.y);
}

void micros_console_set_foreground_color(micros_console_color color)
{
    foreground_color = color;
}

void micros_console_set_background_color(micros_console_color color)
{
    background_color = color;
}

void micros_console_set_cursor_position(micros_console_position position)
{
    micros_interrupt_2a(0x13, position.x, position.y);
}

void micros_console_get_cursor_position(micros_console_position *micros_console_position)
{
    micros_interrupt_1a(0x14, (uint32_t)micros_console_position);
}

void micros_console_clear()
{
    micros_interrupt_0a(0x15);
}