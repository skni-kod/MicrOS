#include "vga.h"

// Position on screen
vga_screen_pos vga_cursor_pos;
volatile screen * const vga_video = (volatile screen*)(VGA_BASE_ADDR);
uint8_t vga_current_printing_screen;
uint16_t vga_screen_offset;

void vga_init()
{
    vga_cursor_pos.x = 0;
    vga_cursor_pos.y = 0;
    vga_current_printing_screen = 0;
    vga_screen_offset = 0;
    vga_clear_screen();
    vga_update_cursor_struct(vga_cursor_pos);
}

void vga_printchar(char c)
{
    vga_printchar_color(c, 0);
}

void vga_printchar_color(char c, vga_color * color)
{
    uint16_t pos = vga_calcualte_position_with_offset(vga_cursor_pos.x, vga_cursor_pos.y);

    if(c != '\n')
    {
        vga_video[pos].c.code = c;
        if(color != 0)
        {
            vga_video[pos].c.color = *color;
        }
        vga_cursor_pos.x += 1;
        if(vga_cursor_pos.x == VGA_SCREEN_COLUMNS)
        {
            vga_newline();
        }
    }
    else
    {
        vga_newline();
    }
    vga_update_cursor_struct(vga_cursor_pos);
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

void vga_printstring_color(char * str, vga_color * color)
{
    char* ptr = str;
    while(*ptr != 0)
    {
        vga_printchar_color(*ptr, color);
        ptr++;
    }
}

void vga_set_char(uint16_t x, uint16_t y, char c)
{
    uint16_t pos = vga_calcualte_position_with_offset(x, y);
    vga_video[pos].c.code = c;
}

void vga_set_char_struct(vga_screen_pos spos, char c)
{
    vga_set_char(spos.x, spos.y, c);
}

char vga_get_char(uint16_t x, uint16_t y)
{
    uint16_t pos = vga_calcualte_position_with_offset(x, y);
    return vga_video[pos].c.code;
}

char vga_get_char_struct(vga_screen_pos spos)
{
    return vga_get_char(spos.x, spos.y);
}

void vga_set_color(uint16_t x, uint16_t y, vga_color col)
{
    uint16_t pos = vga_calcualte_position_with_offset(x, y);
    vga_video[pos].c.color = col;
}

void vga_set_color_struct(vga_screen_pos spos, vga_color col)
{
    vga_set_color(spos.x, spos.y, col);
}

vga_color vga_get_color(uint16_t x, uint16_t y)
{
    uint16_t pos = vga_calcualte_position_with_offset(x, y);
    return vga_video[pos].c.color;
}

vga_color vga_get_color_struct(vga_screen_pos spos)
{
    return vga_get_color(spos.x, spos.y);
}

void vga_set_character(uint16_t x, uint16_t y, vga_character c)
{
    uint16_t pos = vga_calcualte_position_with_offset(x, y);
    vga_video[pos].c = c;
}

void vga_set_character_struct(vga_screen_pos spos, vga_character c)
{
    vga_set_character(spos.x, spos.y, c);
}

vga_character vga_get_character(uint16_t x, uint16_t y)
{
    uint16_t pos = vga_calcualte_position_with_offset(x, y);
    return vga_video[pos].c;
}

vga_character vga_get_character_struct(vga_screen_pos spos)
{
    return vga_get_character(spos.x, spos.y);
}

void vga_set_cursor_pos(uint16_t x, uint16_t y)
{
    vga_cursor_pos.x = x;
    vga_cursor_pos.y = y;
}

void vga_set_cursor_pos_struct(vga_screen_pos spos)
{
    vga_cursor_pos = spos;
}

vga_screen_pos vga_get_cursor_pos()
{
    return vga_cursor_pos;
}

void vga_clear_screen()
{
    vga_color_without_blink col = { .background = VGA_COLOR_BLACK, .letter = VGA_COLOR_LIGHT_GRAY };
    // Clear all rows
    for(uint16_t i = 0; i < VGA_SCREEN_ROWS; ++i)
    {
        // Clear all lines
        for(uint16_t j = 0; j < VGA_SCREEN_COLUMNS; ++j)
        {
            uint16_t pos = vga_calcualte_position_with_offset(j, i);
            // Clear 
            vga_video[pos].c.code = 0;
            vga_video[pos].c.color.color_without_blink = col;
        }
    }

    vga_cursor_pos.x = 0;
    vga_cursor_pos.y = 0;
    vga_update_cursor_struct(vga_cursor_pos);
}

void vga_change_printing_screen(uint8_t a)
{
    if(a < VGA_MAX_SCREENS)
    {
        vga_current_printing_screen = a;
        vga_screen_offset = VGA_SCREEN_OFFSET * (uint16_t)a;
    }
}

void vga_copy_screen(uint8_t from, uint8_t to)
{
    if(from < VGA_MAX_SCREENS && to < VGA_MAX_SCREENS)
    {
        uint16_t offset_from = VGA_SCREEN_OFFSET * (uint16_t)from;
        uint16_t offset_to = VGA_SCREEN_OFFSET * (uint16_t)to;

        // Copy all rows
        for(uint16_t i = 0; i < VGA_SCREEN_ROWS; ++i)
        {
            // Copy all lines
            for(uint16_t j = 0; j < VGA_SCREEN_COLUMNS; ++j)
            {
                uint16_t pos = vga_calcualte_position_without_offset(j, i);
                // Copy 
                vga_video[pos + offset_to].value = vga_video[pos + offset_from].value;
            }
        }
    }
}

void vga_cursor_on()
{
    vga_enable_cursor(14, 15);
}

void vga_cursor_off()
{
    vga_disable_cursor();
}

void vga_newline()
{
    vga_cursor_pos.x = 0;
    vga_cursor_pos.y++;
    // When we reach end of screen
    if(vga_cursor_pos.y == VGA_SCREEN_ROWS)
    {
        // To current line we copy next line
        for(uint16_t i = 0; i < VGA_SCREEN_ROWS - 1; ++i)
        {
            for(uint16_t j = 0; j < VGA_SCREEN_COLUMNS; ++j)
            {
                uint16_t pos = vga_calcualte_position_with_offset(j, i);
                // Copy byte from next line
                vga_video[pos] = vga_video[pos + VGA_SCREEN_COLUMNS];
            }
        }
        vga_cursor_pos.y = VGA_SCREEN_ROWS - 1;

        // Clear last line
        vga_color_without_blink col = { .background = VGA_COLOR_BLACK, .letter = VGA_COLOR_LIGHT_GRAY };
        for(uint16_t i = 0; i < VGA_SCREEN_COLUMNS; ++i)
        {
            uint16_t pos = vga_calcualte_position_with_offset(i, vga_cursor_pos.y);
            // Clear 
            vga_video[pos].c.code = 0;
            vga_video[pos].c.color.color_without_blink = col;
        }
    }
}

uint16_t vga_calcualte_position_with_offset(uint16_t x, uint16_t y)
{
    return (x + y * VGA_SCREEN_COLUMNS) + vga_screen_offset;
}

uint16_t vga_calcualte_position_without_offset(uint16_t x, uint16_t y)
{
    return x + y * VGA_SCREEN_COLUMNS;
}

void vga_enable_cursor(uint8_t cursor_start, uint8_t cursor_end)
{
    outb(0x3D4, 0x0A);
	outb(0x3D5, (inb(0x3D5) & 0xC0) | cursor_start);
 
	outb(0x3D4, 0x0B);
	outb(0x3D5, (inb(0x3D5) & 0xE0) | cursor_end);
}

void vga_disable_cursor()
{
	outb(0x3D4, 0x0A);
	outb(0x3D5, 0x20);
}

void vga_update_cursor(uint16_t x, uint16_t y)
{
	uint16_t pos = vga_calcualte_position_without_offset(x, y);
 
	outb(0x3D4, 0x0F);
	outb(0x3D5, (uint8_t) (pos & 0xFF));
	outb(0x3D4, 0x0E);
	outb(0x3D5, (uint8_t) ((pos >> 8) & 0xFF));
}

void vga_update_cursor_struct(vga_screen_pos pos)
{
    vga_update_cursor(pos.x, pos.y);
}