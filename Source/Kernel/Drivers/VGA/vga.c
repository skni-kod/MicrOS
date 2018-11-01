#include "vga.h"

// Position on screen
struct screen_pos cursor_pos = { .x = 0, .y = 0 };
volatile union screen * const video = (volatile union screen*)(VGA_BASE_ADDR);

void vga_printchar(char c)
{
    vga_printchar_color(c, 0);
}

void vga_printchar_color(char c, union vga_color * color)
{
    int pos = cursor_pos.x + cursor_pos.y * VGA_SCREEN_COLUMNS;

    if(c != '\n')
    {
        video[pos].c.code = c;
        if(color != 0)
        {
            video[pos].c.color = *color;
        }
        cursor_pos.x += 1;
        if(cursor_pos.x == VGA_SCREEN_COLUMNS)
        {
            vga_newline();
        }
    }
    else
    {
        vga_newline();
    }
}

void vga_printstring(char * str)
{
    char* ptr = str;
    while(*ptr != 0)
    {
        vga_printchar(*ptr);
        ptr++;
    }
}

void vga_printstring_color(char * str, union vga_color * color)
{
    char* ptr = str;
    while(*ptr != 0)
    {
        vga_printchar(*ptr, color);
        ptr++;
    }
}

void vga_set_char(int x, int y, char c)
{
    int pos = x + y * VGA_SCREEN_COLUMNS;
    video[pos].c.code = c;
}

void vga_set_char_struct(struct screen_pos spos, char c)
{
    int pos = spos.x + spos.y * VGA_SCREEN_COLUMNS;
    video[pos].c.code = c;
}

char vga_get_char(int x, int y)
{
    int pos = x + y * VGA_SCREEN_COLUMNS;
    return video[pos].c.code;
}

char vga_get_char_struct(struct screen_pos spos)
{
    int pos = spos.x + spos.y * VGA_SCREEN_COLUMNS;
    return video[pos].c.code;
}

void vga_set_color(int x, int y, union vga_color col)
{
    int pos = x + y * VGA_SCREEN_COLUMNS;
    video[pos].c.color = col;
}

void vga_set_color_struct(struct screen_pos spos, union vga_color col)
{
    int pos = spos.x + spos.y * VGA_SCREEN_COLUMNS;
    video[pos].c.color = col;
}

union vga_color vga_get_color(int x, int y)
{
    int pos = x + y * VGA_SCREEN_COLUMNS;
    return video[pos].c.color;
}

union vga_color vga_get_color_struct(struct screen_pos spos)
{
    int pos = spos.x + spos.y * VGA_SCREEN_COLUMNS;
    return video[pos].c.color;
}

void vga_set_character(int x, int y, struct vga_character c)
{
    int pos = x + y * VGA_SCREEN_COLUMNS;
    video[pos].c = c;
}

void vga_set_character_struct(struct screen_pos spos, struct vga_character c)
{
    int pos = spos.x + spos.y * VGA_SCREEN_COLUMNS;
    video[pos].c = c;
}

struct vga_character vga_get_character(int x, int y)
{
    int pos = x + y * VGA_SCREEN_COLUMNS;
    return video[pos].c;
}

struct vga_character vga_get_character_struct(struct screen_pos spos)
{
    int pos = spos.x + spos.y * VGA_SCREEN_COLUMNS;
    return video[pos].c;
}

void vga_set_cursor_pos(int x, int y)
{
    cursor_pos.x = x;
    cursor_pos.y = y;
}

void vga_set_cursor_pos_struct(struct screen_pos spos)
{
    cursor_pos = spos;
}

struct screen_pos vga_get_cursor_pos()
{
    return cursor_pos;
}

void vga_clear_screen()
{
    struct vga_color_without_blink col = { .background = VGA_COLOR_BLACK, .letter = VGA_COLOR_LIGHT_GRAY };
    // Clear all rows
    for(int i = 0; i < VGA_SCREEN_ROWS; ++i)
    {
        // Clear all lines
        for(int j = 0; j < VGA_SCREEN_COLUMNS; ++j)
        {
            int pos = j + i * VGA_SCREEN_COLUMNS;
            // Clear 
            video[pos].c.code = 0;
            video[pos].c.color.vga_color_without_blink = col;
        }
    }

    cursor_pos.x = 0;
    cursor_pos.y = 0;
}

void vga_newline()
{
    cursor_pos.x = 0;
    cursor_pos.y++;
    // When we reach end of screen
    if(cursor_pos.y == VGA_SCREEN_ROWS)
    {
        // To current line we copy next line
        for(int i = 0; i < VGA_SCREEN_ROWS - 1; ++i)
        {
            for(int j = 0; j < VGA_SCREEN_COLUMNS; ++j)
            {
                int pos = j + i * VGA_SCREEN_COLUMNS;
                // Copy byte from next line
                video[pos] = video[pos + VGA_SCREEN_COLUMNS];
            }
        }
        cursor_pos.y = VGA_SCREEN_ROWS - 1;

        // Clear last line
        struct vga_color_without_blink col = { .background = VGA_COLOR_BLACK, .letter = VGA_COLOR_LIGHT_GRAY };
        for(int i = 0; i < VGA_SCREEN_COLUMNS; ++i)
        {
            int pos = i + cursor_pos.y * VGA_SCREEN_COLUMNS; 
            // Clear 
            video[pos].c.code = 0;
            video[pos].c.color.vga_color_without_blink = col;
        }
    }
}