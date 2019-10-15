#ifndef VGA_H
#define VGA_H

#include <stdint.h>
#include "assembly/io.h"

// Legacy shit, please switch to 03h defines or proper to current graphic mode
// TODO: Remove this
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

// Modes

enum VGA_TEXT_MODES
{
    VGA_MODE_00H = 0x00,
    VGA_MODE_01H = 0x01,
    VGA_MODE_02H = 0x02,
    VGA_MODE_03H = 0x03,
    VGA_MODE_07H = 0x07
};

// Mode 00h

//! The base memory address for mode 00h.
#define VGA_MODE_00H_BASE_ADDR 0xC00B8000
//! The number of columns in mode 00h.
#define VGA_MODE_00H_SCREEN_COLUMNS 40
//! The number of rows in mode 00h.
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

#define VGA_MODE_00H_MAX_SCREENS 8
#define VGA_MODE_00H_SCREEN_OFFSET 4000

// Mode 01h

//! The base memory address for mode 01h.
#define VGA_MODE_01H_BASE_ADDR 0xC00B8000
//! The number of columns in mode 01h.
#define VGA_MODE_01H_SCREEN_COLUMNS 40
//! The number of rows in mode 01h.
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

#define VGA_MODE_01H_MAX_SCREENS 8
#define VGA_MODE_01H_SCREEN_OFFSET 4000

// Mode 02h

//! The base memory address for mode 02h.
#define VGA_MODE_02H_BASE_ADDR 0xC00B8000
//! The number of columns in mode 02h.
#define VGA_MODE_02H_SCREEN_COLUMNS 80
//! The number of rows in mode 02h.
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

#define VGA_MODE_02H_MAX_SCREENS 8
#define VGA_MODE_02H_SCREEN_OFFSET 4000

// Mode 03h

//! The base memory address for mode 03h.
#define VGA_MODE_03H_BASE_ADDR 0xC00B8000
//! The number of columns in mode 03h.
#define VGA_MODE_03H_SCREEN_COLUMNS 80
//! The number of rows in mode 03h.
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

#define VGA_MODE_03H_MAX_SCREENS 8
#define VGA_MODE_03H_SCREEN_OFFSET 4000

// Mode 07h

//! The base memory address for mode 07h.
#define VGA_MODE_07H_BASE_ADDR 0xC00B0000
//! The number of columns in mode 07h.
#define VGA_MODE_07H_SCREEN_COLUMNS 80
//! The number of rows in mode 07h.
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

#define VGA_MODE_07H_MAX_SCREENS 1
#define VGA_MODE_07H_SCREEN_OFFSET 4000


// TODO: Change some names  
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

//! Initialization of VGA.
/*!
    Sets internal variables to given text mode and clear all screens.
    \param mode Text mode you want to switch.
    \return 1 is succes, 0 is failure.
*/
uint8_t vga_init(uint8_t mode);

//! Print character on screen.
/*!
    Print character on screen without changing color of character and background.
    \param character Character.
*/
void vga_printchar(char character);

//! Print character on screen.
/*!
    Print character on screen with given color of character and background.
    \param character Character.
    \param color Color.
*/
void vga_printchar_color(char character, vga_color *color);

//! Print string on screen.
/*!
    Print string on screen without changing color of characters and background.
    \param str String.
*/
void vga_printstring(const char *str);

//! Print string on screen.
/*!
    Print string on screen with given color of characters and background.
    \param str String.
    \param color Color.
*/
void vga_printstring_color(const char *str, vga_color *color);

//! Set character on given point on screen.
/*!
    Set character on given point on screen without changing color of character and background.
    \param x x coordinate.
    \param y y coordinate.
    \param character Character.
*/
void vga_set_char(uint16_t x, uint16_t y, char character);

//! Set character on given point on screen.
/*!
    Set character on given point on screen without changing color of character and background.
    \param spos Position on screen.
    \param character Character.
*/
void vga_set_char_struct(vga_screen_pos spos, char character);

//! Get character from given point on screen.
/*!
    Get character on given point on screen.
    \param x x coordinate.
    \param y y coordinate.
    \return Character.
*/
char vga_get_char(uint16_t x, uint16_t y);

//! Get character from given point on screen.
/*!
    Get character on given point on screen.
    \param spos Position on screen.
    \return Character.
*/
char vga_get_char_struct(vga_screen_pos spos);

//! Set character color on given point on screen.
/*!
    Set character color on given point on screen without changing letter.
    \param x x coordinate.
    \param y y coordinate.
    \param col Color.
*/
void vga_set_color(uint16_t x, uint16_t y, vga_color col);

//! Set character color on given point on screen.
/*!
    Set character color on given point on screen without changing letter.
    \param spos Position on screen.
    \param col Color.
*/
void vga_set_color_struct(vga_screen_pos spos, vga_color col);

//! Get character color from given point on screen.
/*!
    Get character color on given point on screen.
    \param x x coordinate.
    \param y y coordinate.
    \return Color.
*/
union vga_color vga_get_color(uint16_t x, uint16_t y);

//! Get character color from given point on screen.
/*!
    Get character color on given point on screen.
    \param spos Position on screen.
    \return Color.
*/
union vga_color vga_get_color_struct(vga_screen_pos spos);

//! Set character and character color on given point on screen.
/*!
    Set character and character color on given point on screen.
    \param x x coordinate.
    \param y y coordinate.
    \param character Character and character color.
*/
void vga_set_character(uint16_t x, uint16_t y, vga_character character);

//! Set character and character color on given point on screen.
/*!
    Set character and character color on given point on screen.
    \param spos Position on screen.
    \param character Character and character color.
*/
void vga_set_character_struct(vga_screen_pos spos, vga_character character);

//! Get character and character color color from given point on screen.
/*!
    Get character and character color color on given point on screen.
    \param x x coordinate.
    \param y y coordinate.
    \return Character and character color.
*/
vga_character vga_get_character(uint16_t x, uint16_t y);

//! Get character and character color color from given point on screen.
/*!
    Get character and character color color on given point on screen.
    \param spos Position on screen.
    \return Character and character color.
*/
vga_character vga_get_character_struct(vga_screen_pos spos);

//! Set cursor position.
/*!
    Set cursor position on screen.
    \param x x coordinate.
    \param y y coordinate.
*/
void vga_set_cursor_pos(uint16_t x, uint16_t y);

//! Set cursor position.
/*!
    Set cursor position on screen.
    \param spos Position on screen.
*/
void vga_set_cursor_pos_struct(vga_screen_pos spos);

//! Get cursor position.
/*!
    Get cursor position on screen.
    \return Position on screen.
*/
vga_screen_pos vga_get_cursor_pos();

//! Clear screen.
/*!
    Clears letters and colors. Set colors to default.
*/
void vga_clear_screen();

//! Clear given screen.
/*!
    Clears letters and colors from given screen. Set colors to default.
    \param screen Screen to clear. Value valid for current text mode.
*/
void vga_clear_given_screen(uint8_t screen);

//! Clear all screen.
/*!
    Clears letters and colors from all screens. Set colors to default.
*/
void vga_clear_all_screens();

//! Change printing screen.
/*!
    Change screen where text are printing.
    /param screen New screen for printing. Value valid for current text mode.
*/
void vga_change_printing_screen(uint8_t screen);

//! Copy printing screen.
/*!
    Copy screen from one to another.
    /param from Screen to copy. Value valid for current text mode.
    /param to Destination screen. Value valid for current text mode.
*/
void vga_copy_screen(uint8_t from, uint8_t to);

//! Turn on cursor.
/*!
    Turn on cursor with default size and position.
    Copy screen from one to another.
*/
void vga_cursor_on();

//! Turn off cursor.
/*!
*/
void vga_cursor_off();

// Helpers

//! Print new line.
/*!
*/
void vga_newline();

//! Calculate position with offset.
/*!
    Calculate position on screen takes into account the current offset. Which means function return value that added to base address will give you position of current character on current screen in memory.
    \param x x coordinate.
    \param y y coordinate.
    \return Calculated position on screen.
*/
uint16_t vga_calcualte_position_with_offset(uint16_t x, uint16_t y);

//! Calculate position without offset.
/*!
    Calculate position on screen doesn't take into account the current offset. Which means function return value that added to base address will give you position of current character on first screen in memory.
    \param x x coordinate.
    \param y y coordinate.
    \return Calculated position on screen.
*/
uint16_t vga_calcualte_position_without_offset(uint16_t x, uint16_t y);

//! Turn on cursor.
/*!
    Turn on cursor with given size and position.
    \param cursor_start Scanline where cursor should start.
    \param cursor_end Scanline where cursor should end.
*/
void vga_enable_cursor(uint8_t cursor_start, uint8_t cursor_end);

//! Disable cursor.
/*!
*/
void vga_disable_cursor();

//! Update cursor.
/*!
    Move cursor to given position.
    \param x x coordinate.
    \param y y coordinate.
*/
void vga_update_cursor(uint16_t x, uint16_t y);

//! Update cursor.
/*!
    Move cursor to given position.
    \param pos Position on screen.
*/
void vga_update_cursor_struct(vga_screen_pos pos);

//! Get default terminal color.
/*!
    Get default terminal color for mode. 
    \param vga_mode Current mode of graphic card.
    \return Default terminal letter and background color. When wrong mode given returns default color for mode 3h.
*/
vga_color vga_get_default_terminal_color(uint8_t vga_current_mode);

#endif