#ifndef VGA_H
#define VGA_H

#include <stdint.h>
#include "assembly/io.h"

// Legacy shit, please switch to 03h defines or proper to current graphic mode

#define VGA_BASE_ADDR 0xC00B8000
#define VGA_SCREEN_COLUMNS 80
#define VGA_SCREEN_ROWS 25

#define VGA_COLOR_BLACK 0x00
#define VGA_COLOR_BLUE 0x01
#define VGA_COLOR_GREEN 0x02
#define VGA_COLOR_CYAN 0x03
#define VGA_COLOR_RED 0x04
#define VGA_COLOR_MAGENTA 0x05
#define VGA_COLOR_BROWN 0x06
#define VGA_COLOR_LIGHT_GRAY 0x07
#define VGA_COLOR_DARK_GRAY 0x08
#define VGA_COLOR_LIGHT_BLUE 0x09
#define VGA_COLOR_LIGHT_GREEN 0x0A
#define VGA_COLOR_LIGHT_CYAN 0x0B
#define VGA_COLOR_LIGHT_RED 0x0C
#define VGA_COLOR_LIGHT_MAGENTA 0x0D
#define VGA_COLOR_YELLOW 0x0E
#define VGA_COLOR_WHITE 0x0F
#define VGA_NOT_BLINK 0x00
#define VGA_BLINK 0x01

// Mode 00h

#define VGA_MODE_00H_BASE_ADDR 0xC00B8000
#define VGA_MODE_00H_SCREEN_COLUMNS 40
#define VGA_MODE_00H_SCREEN_ROWS 25

#define VGA_MODE_00H_COLOR_BLACK 0x00
#define VGA_MODE_00H_COLOR_BLUE 0x01
#define VGA_MODE_00H_COLOR_GREEN 0x02
#define VGA_MODE_00H_COLOR_CYAN 0x03
#define VGA_MODE_00H_COLOR_RED 0x04
#define VGA_MODE_00H_COLOR_MAGENTA 0x05
#define VGA_MODE_00H_COLOR_BROWN 0x06
#define VGA_MODE_00H_COLOR_LIGHT_GRAY 0x07
#define VGA_MODE_00H_COLOR_DARK_GRAY 0x08
#define VGA_MODE_00H_COLOR_LIGHT_BLUE 0x09
#define VGA_MODE_00H_COLOR_LIGHT_GREEN 0x0A
#define VGA_MODE_00H_COLOR_LIGHT_CYAN 0x0B
#define VGA_MODE_00H_COLOR_LIGHT_RED 0x0C
#define VGA_MODE_00H_COLOR_LIGHT_MAGENTA 0x0D
#define VGA_MODE_00H_COLOR_YELLOW 0x0E
#define VGA_MODE_00H_COLOR_WHITE 0x0F
#define VGA_MODE_00H_NOT_BLINK 0x00
#define VGA_MODE_00H_BLINK 0x01

// Mode 01h

#define VGA_MODE_01H_BASE_ADDR 0xC00B8000
#define VGA_MODE_01H_SCREEN_COLUMNS 40
#define VGA_MODE_01H_SCREEN_ROWS 25

#define VGA_MODE_01H_COLOR_BLACK 0x00
#define VGA_MODE_01H_COLOR_BLUE 0x01
#define VGA_MODE_01H_COLOR_GREEN 0x02
#define VGA_MODE_01H_COLOR_CYAN 0x03
#define VGA_MODE_01H_COLOR_RED 0x04
#define VGA_MODE_01H_COLOR_MAGENTA 0x05
#define VGA_MODE_01H_COLOR_BROWN 0x06
#define VGA_MODE_01H_COLOR_LIGHT_GRAY 0x07
#define VGA_MODE_01H_COLOR_DARK_GRAY 0x08
#define VGA_MODE_01H_COLOR_LIGHT_BLUE 0x09
#define VGA_MODE_01H_COLOR_LIGHT_GREEN 0x0A
#define VGA_MODE_01H_COLOR_LIGHT_CYAN 0x0B
#define VGA_MODE_01H_COLOR_LIGHT_RED 0x0C
#define VGA_MODE_01H_COLOR_LIGHT_MAGENTA 0x0D
#define VGA_MODE_01H_COLOR_YELLOW 0x0E
#define VGA_MODE_01H_COLOR_WHITE 0x0F
#define VGA_MODE_01H_NOT_BLINK 0x00
#define VGA_MODE_01H_BLINK 0x01

// Mode 02h

#define VGA_MODE_02H_BASE_ADDR 0xC00B8000
#define VGA_MODE_02H_SCREEN_COLUMNS 80
#define VGA_MODE_02H_SCREEN_ROWS 25

#define VGA_MODE_02H_COLOR_BLACK 0x00
#define VGA_MODE_02H_COLOR_BLUE 0x01
#define VGA_MODE_02H_COLOR_GREEN 0x02
#define VGA_MODE_02H_COLOR_CYAN 0x03
#define VGA_MODE_02H_COLOR_RED 0x04
#define VGA_MODE_02H_COLOR_MAGENTA 0x05
#define VGA_MODE_02H_COLOR_BROWN 0x06
#define VGA_MODE_02H_COLOR_LIGHT_GRAY 0x07
#define VGA_MODE_02H_COLOR_DARK_GRAY 0x08
#define VGA_MODE_02H_COLOR_LIGHT_BLUE 0x09
#define VGA_MODE_02H_COLOR_LIGHT_GREEN 0x0A
#define VGA_MODE_02H_COLOR_LIGHT_CYAN 0x0B
#define VGA_MODE_02H_COLOR_LIGHT_RED 0x0C
#define VGA_MODE_02H_COLOR_LIGHT_MAGENTA 0x0D
#define VGA_MODE_02H_COLOR_YELLOW 0x0E
#define VGA_MODE_02H_COLOR_WHITE 0x0F
#define VGA_MODE_02H_NOT_BLINK 0x00
#define VGA_MODE_02H_BLINK 0x01

// Mode 03h

#define VGA_MODE_03H_BASE_ADDR 0xC00B8000
#define VGA_MODE_03H_SCREEN_COLUMNS 80
#define VGA_MODE_03H_SCREEN_ROWS 25

#define VGA_MODE_03H_COLOR_BLACK 0x00
#define VGA_MODE_03H_COLOR_BLUE 0x01
#define VGA_MODE_03H_COLOR_GREEN 0x02
#define VGA_MODE_03H_COLOR_CYAN 0x03
#define VGA_MODE_03H_COLOR_RED 0x04
#define VGA_MODE_03H_COLOR_MAGENTA 0x05
#define VGA_MODE_03H_COLOR_BROWN 0x06
#define VGA_MODE_03H_COLOR_LIGHT_GRAY 0x07
#define VGA_MODE_03H_COLOR_DARK_GRAY 0x08
#define VGA_MODE_03H_COLOR_LIGHT_BLUE 0x09
#define VGA_MODE_03H_COLOR_LIGHT_GREEN 0x0A
#define VGA_MODE_03H_COLOR_LIGHT_CYAN 0x0B
#define VGA_MODE_03H_COLOR_LIGHT_RED 0x0C
#define VGA_MODE_03H_COLOR_LIGHT_MAGENTA 0x0D
#define VGA_MODE_03H_COLOR_YELLOW 0x0E
#define VGA_MODE_03H_COLOR_WHITE 0x0F
#define VGA_MODE_03H_NOT_BLINK 0x00
#define VGA_MODE_03H_BLINK 0x01

// Mode 07h

#define VGA_MODE_07H_BASE_ADDR 0xC00B0000
#define VGA_MODE_07H_SCREEN_COLUMNS 80
#define VGA_MODE_07H_SCREEN_ROWS 25

#define VGA_MODE_07H_COLOR_BLACK 0x00
#define VGA_MODE_07H_COLOR_BLUE 0x01
#define VGA_MODE_07H_COLOR_GREEN 0x02
#define VGA_MODE_07H_COLOR_CYAN 0x03
#define VGA_MODE_07H_COLOR_RED 0x04
#define VGA_MODE_07H_COLOR_MAGENTA 0x05
#define VGA_MODE_07H_COLOR_BROWN 0x06
#define VGA_MODE_07H_COLOR_LIGHT_GRAY 0x07
#define VGA_MODE_07H_COLOR_DARK_GRAY 0x08
#define VGA_MODE_07H_COLOR_LIGHT_BLUE 0x09
#define VGA_MODE_07H_COLOR_LIGHT_GREEN 0x0A
#define VGA_MODE_07H_COLOR_LIGHT_CYAN 0x0B
#define VGA_MODE_07H_COLOR_LIGHT_RED 0x0C
#define VGA_MODE_07H_COLOR_LIGHT_MAGENTA 0x0D
#define VGA_MODE_07H_COLOR_YELLOW 0x0E
#define VGA_MODE_07H_COLOR_WHITE 0x0F
#define VGA_MODE_07H_NOT_BLINK 0x00
#define VGA_MODE_07H_BLINK 0x01

#define VGA_MAX_SCREENS 8
#define VGA_SCREEN_OFFSET 4000

// Position on screen
typedef struct vga_screen_pos
{
    uint16_t x;
    uint16_t y;
} vga_screen_pos;

// Color with blinking bit
typedef struct vga_color_with_blink
{
    unsigned char letter : 4;
    unsigned char background : 3;
    unsigned char blikning : 1;
} vga_color_with_blink;

// Color without blinking bit
typedef struct vga_color_without_blink
{
    unsigned char letter : 4;
    unsigned char background : 4;
} vga_color_without_blink;

// Color
typedef union vga_color {
    vga_color_with_blink color_with_blink;
    vga_color_without_blink color_without_blink;
} vga_color;

// Character
typedef struct vga_character
{
    unsigned char code;
    vga_color color;
} vga_character;

// Screen
typedef union screen {
    uint16_t value;
    vga_character c;
} screen;

// Functions for users
void vga_init();
void vga_printchar(char c);
void vga_printchar_color(char c, vga_color *color);
void vga_printstring(const char *str);
void vga_printstring_color(const char *str, vga_color *color);
void vga_set_char(uint16_t x, uint16_t y, char c);
void vga_set_char_struct(vga_screen_pos spos, char c);
char vga_get_char(uint16_t x, uint16_t y);
char vga_get_char_struct(vga_screen_pos spos);
void vga_set_color(uint16_t x, uint16_t y, vga_color col);
void vga_set_color_struct(vga_screen_pos spos, vga_color col);
union vga_color vga_get_color(uint16_t x, uint16_t y);
union vga_color vga_get_color_struct(vga_screen_pos spos);
void vga_set_character(uint16_t x, uint16_t y, vga_character c);
void vga_set_character_struct(vga_screen_pos spos, vga_character c);
vga_character vga_get_character(uint16_t x, uint16_t y);
vga_character vga_get_character_struct(vga_screen_pos spos);
void vga_set_cursor_pos(uint16_t x, uint16_t y);
void vga_set_cursor_pos_struct(vga_screen_pos spos);
vga_screen_pos vga_get_cursor_pos();
void vga_clear_screen();
void vga_change_printing_screen(uint8_t a);
void vga_copy_screen(uint8_t from, uint8_t to);
void vga_cursor_on();
void vga_cursor_off();

// Helpers
void vga_newline();
uint16_t vga_calcualte_position_with_offset(uint16_t x, uint16_t y);
uint16_t vga_calcualte_position_without_offset(uint16_t x, uint16_t y);
void vga_enable_cursor(uint8_t cursor_start, uint8_t cursor_end);
void vga_disable_cursor();
void vga_update_cursor(uint16_t x, uint16_t y);
void vga_update_cursor_struct(vga_screen_pos pos);

#endif