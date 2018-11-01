#include "vga.h"

struct screen_pos cursor_pos = { .x = 0, .y = 0 };

void vga_printchar(char c)
{
    vga_printcharcolor(c, 0);
}

void vga_printcharcolor(char c, union vga_color* color)
{
    int pos = cursor_pos.x + cursor_pos.y * VGA_SCREEN_COLUMNS;
    volatile union screen* video = (volatile union screen*)(VGA_BASE_ADDR);

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

void vga_printstring(char* str)
{
    char* ptr = str;
    while(*ptr != 0)
    {
        vga_printchar(*ptr);
        ptr++;
    }
}

void vga_newline()
{
    cursor_pos.x = 0;
    cursor_pos.y++;
    // When we reach end of screen
    if(cursor_pos.y == VGA_SCREEN_ROWS)
    {
        volatile union screen* video = (volatile union screen*)(VGA_BASE_ADDR);
        // To current line we copy next line
        for(int i = 0; i < VGA_SCREEN_ROWS - 1; ++i)
        {
            // Every column has 2 bytes (character and color)
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