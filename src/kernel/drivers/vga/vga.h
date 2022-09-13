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

//! Enum represent available VGA text modes.
enum VGA_TEXT_MODES
{
    VGA_MODE_00H = 0x00, /*!< VGA mode 00h. */
    VGA_MODE_01H = 0x01, /*!< VGA mode 01h. */
    VGA_MODE_02H = 0x02, /*!< VGA mode 02h. */
    VGA_MODE_03H = 0x03, /*!< VGA mode 03h. */
    VGA_MODE_07H = 0x07 /*!< VGA mode 07h. */
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

//! Struct that defines position on screen.
typedef struct vga_screen_pos
{
    //! Position x on screen.
    uint16_t x;
    //! Position y on screen.
    uint16_t y;
} vga_screen_pos;

//! Color of one character with blinking bit.
/*! Defines color of one letter and it's background in blinking mode. Struct contains 4 bits of letter color,
3 bits of background color and 1 bit for indication of blinking character.*/
typedef struct vga_color_with_blink
{
    //! Color of letter.
    unsigned char letter : 4;
    //! Color of background.
    unsigned char background : 3;
    //! Flag for enable blinking.
    unsigned char blikning : 1;
} vga_color_with_blink;

//! Color of one character without blinking bit.
/*! Defines color of one letter and it's background in non blinking mode. Struct contains 4 bits of letter color,
4 bits of background color.*/
typedef struct vga_color_without_blink
{
    //! Color of letter.
    unsigned char letter : 4;
    //! Color of background.
    unsigned char background : 4;
} vga_color_without_blink;

//! Color of one character.
/*! Union that defines color of one character and it's background. When you use blinking mode you should use
`color_with_blink` field and when you use non blinking mode you should use `color_without_blink` field.*/
typedef union vga_color {
    //! Uses to define color in blinking mode.
    vga_color_with_blink color_with_blink;
    //! Uses to define color in non blinking mode.
    vga_color_without_blink color_without_blink;
    //! Uses to define color as value.
    uint8_t value;
} vga_color;

//! Struct that define one character.
/*! Contains code of letter and it's color.*/
typedef struct vga_character
{
    //! ASCII code of letter.
    unsigned char ascii_code;
    //! Color of letter and background. In blinking mode also include blinking flag.
    vga_color color;
} vga_character;

//! Define one character on screen.
/*! Union contains 16 bit value of field (which include ASCII letter and color data) and vga_character strunc that allow to
access letter and color separately. Table of then can define whole screen.*/
typedef union screen_char {
    //! 16 bit value of field.
    uint16_t value;
    //! Struct contains ASCII code and color data.
    vga_character character;
} screen_char;

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
*/
void vga_cursor_on();

//! Turn off cursor.
/*!
*/
void vga_cursor_off();

//! Print new line.
/*!
*/
void vga_newline();

// Helpers

//! Calculate position with offset.
/*!
    Calculate position on screen takes into account the current offset. Which means function return value that added to base address will give you position of current character on current screen in memory.
    \param x x coordinate.
    \param y y coordinate.
    \return Calculated position on screen.
*/
uint16_t __vga_calcualte_position_with_offset(uint16_t x, uint16_t y);

//! Calculate position without offset.
/*!
    Calculate position on screen doesn't take into account the current offset. Which means function return value that added to base address will give you position of current character on first screen in memory.
    \param x x coordinate.
    \param y y coordinate.
    \return Calculated position on screen.
*/
uint16_t __vga_calcualte_position_without_offset(uint16_t x, uint16_t y);

//! Turn on cursor.
/*!
    Turn on cursor with given size and position.
    \param cursor_start Scanline where cursor should start.
    \param cursor_end Scanline where cursor should end.
*/
void __vga_enable_cursor(uint8_t cursor_start, uint8_t cursor_end);

//! Disable cursor.
/*!
*/
void __vga_disable_cursor();

//! Get default terminal color.
/*!
    Get default terminal color for mode. 
    \param vga_mode Current mode of graphic card.
    \return Default terminal letter and background color. When wrong mode given returns default color for mode 3h.
*/
vga_color __vga_get_default_terminal_color(uint8_t vga_current_mode);

#endif