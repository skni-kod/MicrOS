#include "micros_console.h"

micros_console_color foreground_color = micros_console_color_light_gray;
micros_console_color background_color = micros_console_color_black;

void micros_console_print_char(char c)
{
#ifndef __libk

    micros_interrupt_3a(0x0100, (uint32_t)c, (uint32_t)foreground_color, (uint32_t)background_color);

#else

#endif
}

void micros_console_print_string(const char *str)
{
#ifndef __libk

    micros_interrupt_3a(0x0101, (uint32_t)str, (uint32_t)foreground_color, (uint32_t)background_color);

#else

#endif
}

char micros_console_get_char_at_position(micros_console_position *position)
{
#ifndef __libk

    return (char)micros_interrupt_2a(0x0102, position->x, position->y);

#else

#endif
}

void micros_console_set_foreground_color(micros_console_color color)
{
    foreground_color = color;
}

void micros_console_set_background_color(micros_console_color color)
{
    background_color = color;
}

void micros_console_set_cursor_position(micros_console_position *position)
{
#ifndef __libk

    micros_interrupt_2a(0x0103, position->x, position->y);

#else

#endif
}

void micros_console_get_cursor_position(micros_console_position *position)
{
#ifndef __libk

    micros_interrupt_1a(0x0104, (uint32_t)position);

#else

#endif
}

void micros_console_clear()
{
#ifndef __libk

    micros_interrupt_0a(0x0105);

#else

#endif
}

void micros_console_set_cursor_visibility(bool visibility)
{
#ifndef __libk

    micros_interrupt_1a(0x0106, (uint32_t)visibility);

#else

#endif
}

void micros_console_set_video_mode(uint8_t mode)
{
#ifndef __libk

    micros_interrupt_1a(0x010B, mode);
    
#else

#endif
}

void micros_console_copy_from_buffer(uint8_t *buffer, uint32_t how_many)
{
#ifndef __libk

    micros_interrupt_2a(0x010C, (uint32_t)buffer, how_many);

#else

#endif
}
