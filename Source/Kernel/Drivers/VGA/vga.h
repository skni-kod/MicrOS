#pragma once
#define VGA_BASE_ADDR 0xB8000
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

// Position on screen
struct screen_pos
{
    int x;
    int y;
};

// Color with blinking bit
struct vga_color_with_blink
{
    unsigned char letter :4;
    unsigned char background :3;
    unsigned char blikning :1;
};

// Color without blinking bit
struct vga_color_without_blink
{
    unsigned char letter :4;
    unsigned char background :4;
};

// Color
union vga_color
{
    struct vga_color_with_blink color_with_blink;
    struct vga_color_without_blink vga_color_without_blink;
};

// Character
struct vga_character
{
    unsigned char code;
    union vga_color color;
};

// Screen
union screen
{
    unsigned short value;
    struct vga_character c;
};

// Functions for users
void vga_printchar(char c);
void vga_printchar_color(char c, union vga_color * color);
void vga_printstring(char * str);
void vga_printstring_color(char * str, union vga_color * color);
void vga_set_char(int x, int y, char c);
void vga_set_char_struct(struct screen_pos spos, char c);
char vga_get_char(int x, int y);
char vga_get_char_struct(struct screen_pos spos);
void vga_set_color(int x, int y, union vga_color col);
void vga_set_color_struct(struct screen_pos spos, union vga_color col);
union vga_color vga_get_color(int x, int y);
union vga_color vga_get_color_struct(struct screen_pos spos);
void vga_set_character(int x, int y, struct vga_character c);
void vga_set_character_struct(struct screen_pos spos, struct vga_character c);
struct vga_character vga_get_character(int x, int y);
struct vga_character vga_get_character_struct(struct screen_pos spos);
void vga_set_cursor_pos(int x, int y);
void vga_set_cursor_pos_struct(struct screen_pos spos);
struct screen_pos vga_get_cursor_pos();
void vga_clear_screen();

// Helpers
void vga_newline();