#include "vga.h"

//! Pointer to current screen
/*!
    Points to array of characters
*/ 
volatile screen_char *vga_video;

//! Current video screen.
uint8_t vga_current_printing_screen = 0;

//! Current offset for screen.
/*!
    When we add this to `vga_video` we wil get address of screen we should work on.
*/
uint16_t vga_screen_offset = 0;

//! Current text mode.
uint8_t vga_current_mode = 0;

//! Number of columns screens for current text mode.
uint16_t vga_current_columns = 0;

//! Number of rows screens for current text mode.
uint16_t vga_current_rows = 0;

//! Number of max screens for current text mode.
uint16_t vga_current_max_screens = 0;

//! Offset between screens for current text mode.
uint16_t vga_current_screen_offset = 0;

uint8_t vga_init(uint8_t mode)
{
    switch(mode)
    {
        case VGA_MODE_00H:
            vga_current_mode = mode;
            vga_current_columns = VGA_MODE_00H_SCREEN_COLUMNS;
            vga_current_rows = VGA_MODE_00H_SCREEN_ROWS;
            vga_current_max_screens = VGA_MODE_00H_MAX_SCREENS;
            vga_current_screen_offset = VGA_MODE_00H_SCREEN_OFFSET;
            vga_set_cursor_pos(0, 0);
            vga_current_printing_screen = 0;
            vga_screen_offset = 0;
            vga_video = (volatile screen_char *)(VGA_MODE_00H_BASE_ADDR);
            vga_clear_all_screens();
            return 1;
        case VGA_MODE_01H:
            vga_current_mode = mode;
            vga_current_columns = VGA_MODE_01H_SCREEN_COLUMNS;
            vga_current_rows = VGA_MODE_01H_SCREEN_ROWS;
            vga_current_max_screens = VGA_MODE_01H_MAX_SCREENS;
            vga_current_screen_offset = VGA_MODE_01H_SCREEN_OFFSET;
            vga_set_cursor_pos(0, 0);
            vga_current_printing_screen = 0;
            vga_screen_offset = 0;
            vga_video = (volatile screen_char *)(VGA_MODE_01H_BASE_ADDR);
            vga_clear_all_screens();
            return 1;
        case VGA_MODE_02H:
            vga_current_mode = mode;
            vga_current_columns = VGA_MODE_02H_SCREEN_COLUMNS;
            vga_current_rows = VGA_MODE_02H_SCREEN_ROWS;
            vga_current_max_screens = VGA_MODE_02H_MAX_SCREENS;
            vga_current_screen_offset = VGA_MODE_02H_SCREEN_OFFSET;
            vga_set_cursor_pos(0, 0);
            vga_current_printing_screen = 0;
            vga_screen_offset = 0;
            vga_video = (volatile screen_char *)(VGA_MODE_02H_BASE_ADDR);
            vga_clear_all_screens();
            return 1;
        case VGA_MODE_03H:
            vga_current_mode = mode;
            vga_current_columns = VGA_MODE_03H_SCREEN_COLUMNS;
            vga_current_rows = VGA_MODE_03H_SCREEN_ROWS;
            vga_current_max_screens = VGA_MODE_03H_MAX_SCREENS;
            vga_current_screen_offset = VGA_MODE_03H_SCREEN_OFFSET;
            vga_set_cursor_pos(0, 0);
            vga_current_printing_screen = 0;
            vga_screen_offset = 0;
            vga_video = (volatile screen_char *)(VGA_MODE_03H_BASE_ADDR);
            vga_clear_all_screens();
            return 1;
        case VGA_MODE_07H:
            vga_current_mode = mode;
            vga_current_columns = VGA_MODE_07H_SCREEN_COLUMNS;
            vga_current_rows = VGA_MODE_07H_SCREEN_ROWS;
            vga_current_max_screens = VGA_MODE_07H_MAX_SCREENS;
            vga_current_screen_offset = VGA_MODE_07H_SCREEN_OFFSET;
            vga_set_cursor_pos(0, 0);
            vga_current_printing_screen = 0;
            vga_screen_offset = 0;
            vga_video = (volatile screen_char *)(VGA_MODE_07H_BASE_ADDR);
            vga_clear_all_screens();
            return 1;
        default:
            return -1;
    }
    
}

void vga_printchar(char character)
{
    vga_screen_pos vga_cursor_pos = vga_get_cursor_pos();
    uint16_t pos = __vga_calcualte_position_with_offset(vga_cursor_pos.x, vga_cursor_pos.y);

    if (character != '\n')
    {
        vga_video[pos].character.ascii_code = character;
        vga_video[pos].character.color = __vga_get_default_terminal_color(vga_current_mode);
        vga_cursor_pos.x += 1;
        if (vga_cursor_pos.x == vga_current_columns)
        {
            vga_set_cursor_pos_struct(vga_cursor_pos);
            vga_newline();
            vga_cursor_pos = vga_get_cursor_pos();
        }
    }
    else
    {
        vga_set_cursor_pos_struct(vga_cursor_pos);
        vga_newline();
        vga_cursor_pos = vga_get_cursor_pos();
    }
    vga_set_cursor_pos_struct(vga_cursor_pos);
}

void vga_printchar_color(char character, vga_color *color)
{
    vga_screen_pos vga_cursor_pos = vga_get_cursor_pos();
    uint16_t pos = __vga_calcualte_position_with_offset(vga_cursor_pos.x, vga_cursor_pos.y);

    if (character != '\n')
    {
        vga_video[pos].character.ascii_code = character;
        vga_video[pos].character.color = *color;
        vga_cursor_pos.x += 1;
        if (vga_cursor_pos.x == vga_current_columns)
        {
            vga_set_cursor_pos_struct(vga_cursor_pos);
            vga_newline();
            vga_cursor_pos = vga_get_cursor_pos();
        }
    }
    else
    {
        vga_set_cursor_pos_struct(vga_cursor_pos);
        vga_newline();
        vga_cursor_pos = vga_get_cursor_pos();
    }
    vga_set_cursor_pos_struct(vga_cursor_pos);
}

void vga_printstring(const char *str)
{
    const char *ptr = str;
    while (*ptr != 0)
    {
        vga_printchar(*ptr);
        ptr++;
    }
}

void vga_printstring_color(const char *str, vga_color *color)
{
    const char *ptr = str;
    while (*ptr != 0)
    {
        vga_printchar_color(*ptr, color);
        ptr++;
    }
}

void vga_set_char(uint16_t x, uint16_t y, char character)
{
    uint16_t pos = __vga_calcualte_position_with_offset(x, y);
    vga_video[pos].character.ascii_code = character;
}

void vga_set_char_struct(vga_screen_pos spos, char c)
{
    vga_set_char(spos.x, spos.y, c);
}

char vga_get_char(uint16_t x, uint16_t y)
{
    uint16_t pos = __vga_calcualte_position_with_offset(x, y);
    return vga_video[pos].character.ascii_code;
}

char vga_get_char_struct(vga_screen_pos spos)
{
    return vga_get_char(spos.x, spos.y);
}

void vga_set_color(uint16_t x, uint16_t y, vga_color col)
{
    uint16_t pos = __vga_calcualte_position_with_offset(x, y);
    vga_video[pos].character.color = col;
}

void vga_set_color_struct(vga_screen_pos spos, vga_color col)
{
    vga_set_color(spos.x, spos.y, col);
}

vga_color vga_get_color(uint16_t x, uint16_t y)
{
    uint16_t pos = __vga_calcualte_position_with_offset(x, y);
    return vga_video[pos].character.color;
}

vga_color vga_get_color_struct(vga_screen_pos spos)
{
    return vga_get_color(spos.x, spos.y);
}

void vga_set_character(uint16_t x, uint16_t y, vga_character character)
{
    uint16_t pos = __vga_calcualte_position_with_offset(x, y);
    vga_video[pos].character = character;
}

void vga_set_character_struct(vga_screen_pos spos, vga_character character)
{
    vga_set_character(spos.x, spos.y, character);
}

vga_character vga_get_character(uint16_t x, uint16_t y)
{
    uint16_t pos = __vga_calcualte_position_with_offset(x, y);
    return vga_video[pos].character;
}

vga_character vga_get_character_struct(vga_screen_pos spos)
{
    return vga_get_character(spos.x, spos.y);
}

void vga_set_cursor_pos(uint16_t x, uint16_t y)
{
    uint16_t pos = __vga_calcualte_position_without_offset(x, y);

    io_out_byte(0x3D4, 0x0F);
    io_out_byte(0x3D5, (uint8_t)(pos & 0xFF));
    io_out_byte(0x3D4, 0x0E);
    io_out_byte(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}

void vga_set_cursor_pos_struct(vga_screen_pos spos)
{
    vga_set_cursor_pos(spos.x, spos.y);
}

// TODO: Returned position is not valid after printing some string.
vga_screen_pos vga_get_cursor_pos()
{
    uint16_t pos = 0;
    io_out_byte(0x3D4, 0x0F);
    pos |= io_in_byte(0x3D5);
    io_out_byte(0x3D4, 0x0E);
    pos |= ((uint16_t)io_in_byte(0x3D5)) << 8;
    vga_screen_pos cursor_pos;
    cursor_pos.x = pos % vga_current_columns;
    cursor_pos.y = pos / vga_current_columns;
    return cursor_pos;
}

void vga_clear_screen()
{
    vga_color col = __vga_get_default_terminal_color(vga_current_mode);
    // Clear all rows
    for (uint16_t i = 0; i < vga_current_rows; ++i)
    {
        // Clear all lines
        for (uint16_t j = 0; j < vga_current_columns; ++j)
        {
            uint16_t pos = __vga_calcualte_position_with_offset(j, i);
            // Clear
            vga_video[pos].character.ascii_code = 0;
            vga_video[pos].character.color = col;
        }
    }
    vga_screen_pos vga_cursor_pos;
    vga_cursor_pos.x = 0;
    vga_cursor_pos.y = 0;
    vga_set_cursor_pos_struct(vga_cursor_pos);
}

void vga_clear_given_screen(uint8_t screen)
{
    if (screen < vga_current_max_screens)
    {
        vga_color col = __vga_get_default_terminal_color(vga_current_mode);
        uint16_t offset_to_screen = vga_current_screen_offset * (uint16_t)screen;
        // Clear all rows
        for (uint16_t i = 0; i < vga_current_rows; ++i)
        {
            // Clear all lines
            for (uint16_t j = 0; j < vga_current_columns; ++j)
            {
                uint16_t pos = __vga_calcualte_position_without_offset(j, i);
                // Clear
                vga_video[pos + offset_to_screen].character.ascii_code = 0;
                vga_video[pos + offset_to_screen].character.color = col;
            }
        }
    }
}

void vga_clear_all_screens()
{
    vga_clear_screen();
    for(int i = 1; i < vga_current_max_screens; ++i)
    {
        vga_clear_given_screen(i);
    }
}

void vga_change_printing_screen(uint8_t screen)
{
    if (screen < vga_current_max_screens)
    {
        vga_current_printing_screen = screen;
        vga_screen_offset = vga_current_screen_offset * (uint16_t)screen;
    }
}

void vga_copy_screen(uint8_t from, uint8_t to)
{
    if (from < vga_current_max_screens && to < vga_current_max_screens)
    {
        uint16_t offset_from = vga_current_screen_offset * (uint16_t)from;
        uint16_t offset_to = vga_current_screen_offset * (uint16_t)to;

        // Copy all rows
        for (uint16_t i = 0; i < vga_current_rows; ++i)
        {
            // Copy all lines
            for (uint16_t j = 0; j < vga_current_columns; ++j)
            {
                uint16_t pos = __vga_calcualte_position_without_offset(j, i);
                // Copy
                vga_video[pos + offset_to].value = vga_video[pos + offset_from].value;
            }
        }
    }
}

void vga_cursor_on()
{
    __vga_enable_cursor(14, 15);
}

void vga_cursor_off()
{
    __vga_disable_cursor();
}

void vga_newline()
{
    vga_screen_pos vga_cursor_pos = vga_get_cursor_pos();
    vga_cursor_pos.x = 0;
    vga_cursor_pos.y++;
    // When we reach end of screen
    if (vga_cursor_pos.y == vga_current_rows)
    {
        // To current line we copy next line
        for (uint16_t i = 0; i < vga_current_rows - 1; ++i)
        {
            for (uint16_t j = 0; j < vga_current_columns; ++j)
            {
                uint16_t pos = __vga_calcualte_position_with_offset(j, i);
                // Copy byte from next line
                vga_video[pos] = vga_video[pos + vga_current_columns];
            }
        }
        vga_cursor_pos.y = vga_current_rows - 1;

        // Clear last line
        vga_color col = __vga_get_default_terminal_color(vga_current_mode);
        for (uint16_t i = 0; i < vga_current_columns; ++i)
        {
            uint16_t pos = __vga_calcualte_position_with_offset(i, vga_cursor_pos.y);
            // Clear
            vga_video[pos].character.ascii_code = 0;
            vga_video[pos].character.color = col;
        }
    }
    vga_set_cursor_pos_struct(vga_cursor_pos);
}

uint16_t __vga_calcualte_position_with_offset(uint16_t x, uint16_t y)
{
    return (x + y * vga_current_columns) + vga_screen_offset;
}

uint16_t __vga_calcualte_position_without_offset(uint16_t x, uint16_t y)
{
    return x + y * vga_current_columns;
}

void __vga_enable_cursor(uint8_t cursor_start, uint8_t cursor_end)
{
    io_out_byte(0x3D4, 0x0A);
    io_out_byte(0x3D5, (io_in_byte(0x3D5) & 0xC0) | cursor_start);

    io_out_byte(0x3D4, 0x0B);
    io_out_byte(0x3D5, (io_in_byte(0x3D5) & 0xE0) | cursor_end);
}

void __vga_disable_cursor()
{
    io_out_byte(0x3D4, 0x0A);
    io_out_byte(0x3D5, 0x20);
}

vga_color __vga_get_default_terminal_color(uint8_t vga_current_mode)
{
    vga_color col;
    switch(vga_current_mode)
    {
        case VGA_MODE_00H:
            col.color_without_blink.background = VGA_MODE_00H_COLOR_BLACK;
            col.color_without_blink.letter = VGA_MODE_00H_COLOR_LIGHT_GRAY; 
            break;
        case VGA_MODE_01H:
            col.color_without_blink.background = VGA_MODE_01H_COLOR_BLACK;
            col.color_without_blink.letter = VGA_MODE_01H_COLOR_LIGHT_GRAY;
            break;
        case VGA_MODE_02H:
            col.color_without_blink.background = VGA_MODE_02H_COLOR_BLACK;
            col.color_without_blink.letter = VGA_MODE_02H_COLOR_LIGHT_GRAY;
            break;
        case VGA_MODE_03H:
            col.color_without_blink.background = VGA_MODE_03H_COLOR_BLACK;
            col.color_without_blink.letter = VGA_MODE_03H_COLOR_LIGHT_GRAY;
            break;
        case VGA_MODE_07H:
            col.color_without_blink.background = VGA_MODE_07H_COLOR_BLACK;
            col.color_without_blink.letter = VGA_MODE_07H_COLOR_LIGHT_GRAY;
            break;
        default:
            col.color_without_blink.background = VGA_MODE_03H_COLOR_BLACK;
            col.color_without_blink.letter = VGA_MODE_03H_COLOR_LIGHT_GRAY;
            break;
    }
    return col;
}