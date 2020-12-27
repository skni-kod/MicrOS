#ifndef MODE_01H
#define MODE_01H

#include "stdint.h"

#define MODE01H_WIDTH 40
#define MODE01H_HEIGHT 25

// TODO: implement functions

int8_t mode01h_set_mode();

int8_t mode01h_turn_on_buffer();
int8_t mode01h_turn_off_buffer();
uint8_t mode01h_is_buffer_on();

int8_t mode01h_swap_buffers();

//! Print character on screen for mode 01h.
/*!
    Print character on screen without changing color of character and background.
    \param character Character.
    \return 0 if success, -1 if not implemented.
*/
int8_t mode01h_print_char(char character);

//! Print character on screen for mode 01h.
/*!
    Print character on screen with given color of character and background.
    \param character Character.
    \param color Color.
    \return 0 if success, -1 if not implemented.
*/
int8_t mode01h_print_char_color(char character, uint8_t color);

//! Print string on screen for mode 01h.
/*!
    Print string on screen without changing color of characters and background.
    \param string String.
    \return 0 if success, -1 if not implemented.
*/
int8_t mode01h_print_string(const char* string);

//! Print string on screen for mode 01h.
/*!
    Print string on screen with given color of characters and background.
    \param string String.
    \param color Color.
    \return 0 if success, -1 if not implemented.
*/
int8_t mode01h_print_string_color(const char* string, uint8_t color);

//! Set character on given point on screen for mode 01h.
/*!
    Set character on given point on screen without changing color of character and background.
    \param x x coordinate.
    \param y y coordinate.
    \param character Character.
    \return 0 if success, -1 if not implemented.
*/
int8_t mode01h_set_char(uint16_t x, uint16_t y, char character);

//! Get character from given point on screen for mode 01h.
/*!
    Get character on given point on screen.
    \param x x coordinate.
    \param y y coordinate.
    \param character Return character.
    \return 0 if success, -1 if not implemented.
*/
int8_t mode01h_get_char(uint16_t x, uint16_t y, char* character);

//! Set character color on given point on screen for mode 01h.
/*!
    Set character color on given point on screen without changing letter.
    \param x x coordinate.
    \param y y coordinate.
    \param color Color.
    \return 0 if success, -1 if not implemented.
*/
int8_t mode01h_set_color(uint16_t x, uint16_t y, uint8_t color);

//! Get character color from given point on screen for mode 01h.
/*!
    Get character color on given point on screen.
    \param x x coordinate.
    \param y y coordinate.
    \param color Return color.
    \return 0 if success, -1 if not implemented.
*/
int8_t mode01h_get_color(uint16_t x, uint16_t y, uint8_t* color);

//! Set character and character color on given point on screen for mode 01h.
/*!
    Set character and character color on given point on screen.
    \param x x coordinate.
    \param y y coordinate.
    \param character Character.
    \param color Character color.
    \return 0 if success, -1 if not implemented.
*/
int8_t mode01h_set_char_and_color(uint16_t x, uint16_t y, char character, uint8_t color);

//! Get character and character color color from given point on screen for mode 01h.
/*!
    Get character and character color color on given point on screen.
    \param x x coordinate.
    \param character Return haracter.
    \param color Return character color.
    \return 0 if success, -1 if not implemented.
*/
int8_t mode01h_get_char_and_color(uint16_t x, uint16_t y, char* character, uint8_t* color);

//! Set cursor position for mode 01h.
/*!
    Set cursor position on screen.
    \param x x coordinate.
    \param y y coordinate.
    \return 0 if success, -1 if not implemented.
*/
int8_t mode01h_set_cursor_pos(uint16_t x, uint16_t y);

//! Get cursor position for mode 01h.
/*!
    Get cursor position on screen.
    \param x Return x coordinate.
    \param y Return y coordinate.
    \return 0 if success, -1 if not implemented.
*/
int8_t mode01h_get_cursor_pos(uint16_t* x, uint16_t* y);

//! Turn on cursor for mode 01h.
/*!
    Turn on cursor with default size and position.
    Copy screen from one to another.
    \return 0 if success, -1 if not implemented.
*/
int8_t mode01h_turn_cursor_on();

//! Turn off cursor for mode 01h.
/*!
    \return 0 if success, -1 if not implemented.
*/
int8_t mode01h_turn_cursor_off();

int8_t mode01h_draw_pixel(uint8_t color, uint16_t x, uint16_t y);
int8_t mode01h_draw_line(uint8_t color, uint16_t ax, uint16_t ay, uint16_t bx, uint16_t by);
int8_t mode01h_draw_circle(uint8_t color, uint16_t x, uint16_t y, uint16_t radius);
int8_t mode01h_draw_rectangle(uint8_t color, uint16_t ax, uint16_t ay, uint16_t bx, uint16_t by);
int8_t mode01h_clear_screen();

//! Print character on screen for mode 01h to internal buffer.
/*!
    Print character on screen without changing color of character and background. This function works on internal buffer.
    \param character Character.
    \return 0 if success, -1 if not implemented.
*/
int8_t mode01h_print_char_buffered(char character);

//! Print character on screen for mode 01h to internal buffer.
/*!
    Print character on screen with given color of character and background. This function works on internal buffer.
    \param character Character.
    \param color Color.
    \return 0 if success, -1 if not implemented.
*/
int8_t mode01h_print_char_color_buffered(char character, uint8_t color);

//! Print string on screen for mode 01h to internal buffer.
/*!
    Print string on screen without changing color of characters and background. This function works on internal buffer.
    \param string String.
    \return 0 if success, -1 if not implemented.
*/
int8_t mode01h_print_string_buffered(const char* string);

//! Print string on screen for mode 01h to internal buffer.
/*!
    Print string on screen with given color of characters and background. This function works on internal buffer.
    \param string String.
    \param color Color.
    \return 0 if success, -1 if not implemented.
*/
int8_t mode01h_print_string_color_buffered(const char* string, uint8_t color);

//! Set character on given point on screen for mode 01h to internal buffer.
/*!
    Set character on given point on screen without changing color of character and background. This function works on internal buffer.
    \param x x coordinate.
    \param y y coordinate.
    \param character Character.
    \return 0 if success, -1 if not implemented.
*/
int8_t mode01h_set_char_buffered(uint16_t x, uint16_t y, char character);

//! Get character from given point on screen for mode 01h to internal buffer.
/*!
    Get character on given point on screen. This function works on internal buffer.
    \param x x coordinate.
    \param y y coordinate.
    \param character Return character.
    \return 0 if success, -1 if not implemented.
*/
int8_t mode01h_get_char_buffered(uint16_t x, uint16_t y, char* character);

//! Set character color on given point on screen for mode 01h to internal buffer.
/*!
    Set character color on given point on screen without changing letter. This function works on internal buffer.
    \param x x coordinate.
    \param y y coordinate.
    \param color Color.
    \return 0 if success, -1 if not implemented.
*/
int8_t mode01h_set_color_buffered(uint16_t x, uint16_t y, uint8_t color);

//! Get character color from given point on screen for mode 01h to internal buffer.
/*!
    Get character color on given point on screen. This function works on internal buffer.
    \param x x coordinate.
    \param y y coordinate.
    \param color Return color.
    \return 0 if success, -1 if not implemented.
*/
int8_t mode01h_get_color_buffered(uint16_t x, uint16_t y, uint8_t* color);

//! Set character and character color on given point on screen for mode 01h to internal buffer.
/*!
    Set character and character color on given point on screen. This function works on internal buffer.
    \param x x coordinate.
    \param y y coordinate.
    \param character Character.
    \param color Character color.
    \return 0 if success, -1 if not implemented.
*/
int8_t mode01h_set_char_and_color_buffered(uint16_t x, uint16_t y, char character, uint8_t color);

//! Get character and character color color from given point on screen for mode 01h to internal buffer.
/*!
    Get character and character color color on given point on screen. This function works on internal buffer.
    \param x x coordinate.
    \param character Return haracter.
    \param color Return character color.
    \return 0 if success, -1 if not implemented.
*/
int8_t mode01h_get_char_and_color_buffered(uint16_t x, uint16_t y, char* character, uint8_t* color);

//! Set cursor position for mode 01h to internal buffer.
/*!
    Set cursor position on screen. This function works on internal buffer.
    \param x x coordinate.
    \param y y coordinate.
    \return 0 if success, -1 if not implemented.
*/
int8_t mode01h_set_cursor_pos_buffered(uint16_t x, uint16_t y);

//! Get cursor position for mode 01h to internal buffer.
/*!
    Get cursor position on screen. This function works on internal buffer.
    \param x Return x coordinate.
    \param y Return y coordinate.
    \return 0 if success, -1 if not implemented.
*/
int8_t mode01h_get_cursor_pos_buffered(uint16_t* x, uint16_t* y);

int8_t mode01h_draw_pixel_buffered(uint8_t color, uint16_t x, uint16_t y);
int8_t mode01h_draw_line_buffered(uint8_t color, uint16_t ax, uint16_t ay, uint16_t bx, uint16_t by);
int8_t mode01h_draw_circle_buffered(uint8_t color, uint16_t x, uint16_t y, uint16_t radius);
int8_t mode01h_draw_rectangle_buffered(uint8_t color, uint16_t ax, uint16_t ay, uint16_t bx, uint16_t by);
int8_t mode01h_clear_screen_buffered();

//! Print character on screen for mode 01h to external buffer.
/*!
    Print character on screen without changing color of character and background. This function works on external buffer.
    \param buffer Screen buffer.
    \param mode Mode.
    \param x X coordinate of screen cursor also returns new position of cursor.
    \param y Y coordinate of screen cursor also returns new position of cursor.
    \param character Character.
    \return 0 if success, -1 if not implemented.
*/
int8_t mode01h_print_char_external_buffer(uint8_t* buffer, uint16_t mode, uint16_t* x, uint16_t* y, char character);

//! Print character on screen for mode 01h to external buffer.
/*!
    Print character on screen with given color of character and background. This function works on external buffer.
    \param buffer Screen buffer.
    \param mode Mode.
    \param x X coordinate of screen cursor also returns new position of cursor.
    \param y Y coordinate of screen cursor also returns new position of cursor.
    \param character Character.
    \param color Color.
    \return 0 if success, -1 if not implemented.
*/
int8_t mode01h_print_char_color_external_buffer(uint8_t* buffer, uint16_t mode, uint16_t* x, uint16_t* y, char character, uint8_t color);

//! Print string on screen for mode 01h to external buffer.
/*!
    Print string on screen without changing color of characters and background. This function works on external buffer.
    \param buffer Screen buffer.
    \param mode Mode.
    \param x X coordinate of screen cursor also returns new position of cursor.
    \param y Y coordinate of screen cursor also returns new position of cursor.
    \param string String.
    \return 0 if success, -1 if not implemented.
*/
int8_t mode01h_print_string_external_buffer(uint8_t* buffer, uint16_t mode, uint16_t* x, uint16_t* y, const char* string);

//! Print string on screen for mode 01h to external buffer.
/*!
    Print string on screen with given color of characters and background. This function works on external buffer.
    \param buffer Screen buffer.
    \param mode Mode.
    \param x X coordinate of screen cursor also returns new position of cursor.
    \param y Y coordinate of screen cursor also returns new position of cursor.
    \param string String.
    \param color Color.
    \return 0 if success, -1 if not implemented.
*/
int8_t mode01h_print_string_color_external_buffer(uint8_t* buffer, uint16_t mode, uint16_t* x, uint16_t* y, const char* string, uint8_t color);

//! Set character on given point on screen for mode 01h to external buffer.
/*!
    Set character on given point on screen without changing color of character and background. This function works on external buffer.
    \param buffer Screen buffer.
    \param mode Mode.
    \param x x coordinate.
    \param y y coordinate.
    \param character Character.
    \return 0 if success, -1 if not implemented.
*/
int8_t mode01h_set_char_external_buffer(uint8_t* buffer, uint16_t mode, uint16_t x, uint16_t y, char character);

//! Get character from given point on screen for mode 01h to external buffer.
/*!
    Get character on given point on screen. This function works on external buffer.
    \param buffer Screen buffer.
    \param mode Mode.
    \param x x coordinate.
    \param y y coordinate.
    \param character Return character.
    \return 0 if success, -1 if not implemented.
*/
int8_t mode01h_get_char_external_buffer(uint8_t* buffer, uint16_t mode, uint16_t x, uint16_t y, char* character);

//! Set character color on given point on screen for mode 01h to external buffer.
/*!
    Set character color on given point on screen without changing letter. This function works on external buffer.
    \param buffer Screen buffer.
    \param mode Mode.
    \param x x coordinate.
    \param y y coordinate.
    \param color Color.
    \return 0 if success, -1 if not implemented.
*/
int8_t mode01h_set_color_external_buffer(uint8_t* buffer, uint16_t mode, uint16_t x, uint16_t y, uint8_t color);

//! Get character color from given point on screen for mode 01h to external buffer.
/*!
    Get character color on given point on screen. This function works on external buffer.
    \param buffer Screen buffer.
    \param mode Mode.
    \param x x coordinate.
    \param y y coordinate.
    \param color Return color.
    \return 0 if success, -1 if not implemented.
*/
int8_t mode01h_get_color_external_buffer(uint8_t* buffer, uint16_t mode, uint16_t x, uint16_t y, uint8_t* color);

//! Set character and character color on given point on screen for mode 01h to external buffer.
/*!
    Set character and character color on given point on screen. This function works on external buffer.
    \param buffer Screen buffer.
    \param mode Mode.
    \param x x coordinate.
    \param y y coordinate.
    \param character Character.
    \param color Character color.
    \return 0 if success, -1 if not implemented.
*/
int8_t mode01h_set_char_and_color_external_buffer(uint8_t* buffer, uint16_t mode, uint16_t x, uint16_t y, char character, uint8_t color);

//! Get character and character color color from given point on screen for mode 01h to external buffer.
/*!
    Get character and character color color on given point on screen. This function works on external buffer.
    \param buffer Screen buffer.
    \param mode Mode.
    \param x x coordinate.
    \param character Return haracter.
    \param color Return character color.
    \return 0 if success, -1 if not implemented.
*/
int8_t mode01h_get_char_and_color_external_buffer(uint8_t* buffer, uint16_t mode, uint16_t x, uint16_t y, char* character, uint8_t* color);

int8_t mode01h_draw_pixel_external_buffer(uint8_t* buffer, uint16_t mode, int8_t color, uint16_t x, uint16_t y);
int8_t mode01h_draw_line_external_buffer(uint8_t* buffer, uint16_t mode, uint8_t color, uint16_t ax, uint16_t ay, uint16_t bx, uint16_t by);
int8_t mode01h_draw_circle_external_buffer(uint8_t* buffer, uint16_t mode, uint8_t color, uint16_t x, uint16_t y, uint16_t radius);
int8_t mode01h_draw_rectangle_external_buffer(uint8_t* buffer, uint16_t mode, uint8_t color, uint16_t ax, uint16_t ay, uint16_t bx, uint16_t by);
int8_t mode01h_clear_screen_external_buffer(uint8_t* buffer, uint16_t mode, uint16_t* x, uint16_t* y);

int8_t mode01h_swap_external_buffer(uint8_t* buffer, uint16_t mode);
uint8_t* mode01h_create_external_buffer(uint16_t mode);

// Helpers

//! Helper function to print character on screen for mode 01h on given buffer.
/*!
    Print character on screen without changing color of character and background. This function works on given buffer.
    \param buffer Screen buffer.
    \param mode Mode (only when writes to external buffer).
    \param x X coordinate of screen cursor also returns new position of cursor.
    \param y Y coordinate of screen cursor also returns new position of cursor.
    \param character Character.
    \return 0 if success, -1 if not implemented.
*/
int8_t __mode01h_print_char_buffer(uint16_t* buffer, uint16_t mode, uint16_t* x, uint16_t* y, char character);

//! Helper function to print character on screen for mode 01h on given buffer.
/*!
    Print character on screen with given color of character and background. This function works on given buffer.
    \param buffer Screen buffer.
    \param mode Mode (only when writes to external buffer).
    \param x X coordinate of screen cursor also returns new position of cursor.
    \param y Y coordinate of screen cursor also returns new position of cursor.
    \param character Character.
    \param color Color.
    \return 0 if success, -1 if not implemented.
*/
int8_t __mode01h_print_char_color_buffer(uint16_t* buffer, uint16_t mode, uint16_t* x, uint16_t* y, char character, uint8_t color);

//! Helper function to print string on screen for mode 01h on given buffer.
/*!
    Print string on screen without changing color of characters and background. This function works on given buffer.
    \param buffer Screen buffer.
    \param mode Mode (only when writes to external buffer).
    \param x X coordinate of screen cursor also returns new position of cursor.
    \param y Y coordinate of screen cursor also returns new position of cursor.
    \param string String.
    \return 0 if success, -1 if not implemented.
*/
int8_t __mode01h_print_string_buffer(uint16_t* buffer, uint16_t mode, uint16_t* x, uint16_t* y, const char* string);

//! Helper function to print string on screen for mode 01h on given buffer.
/*!
    Print string on screen with given color of characters and background. This function works on given buffer.
    \param buffer Screen buffer.
    \param mode Mode (only when writes to external buffer).
    \param x X coordinate of screen cursor also returns new position of cursor.
    \param y Y coordinate of screen cursor also returns new position of cursor.
    \param string String.
    \param color Color.
    \return 0 if success, -1 if not implemented.
*/
int8_t __mode01h_print_string_color_buffer(uint16_t* buffer, uint16_t mode, uint16_t* x, uint16_t* y, const char* string, uint8_t color);

//! Helper function to s character on given point on screen for mode 01h on given buffer.
/*!
    Set character on given point on screen without changing color of character and background. This function works on given buffer.
    \param buffer Screen buffer.
    \param mode Mode (only when writes to external buffer).
    \param x x coordinate.
    \param y y coordinate.
    \param character Character.
    \return 0 if success, -1 if not implemented.
*/
int8_t __mode01h_set_char_buffer(uint16_t* buffer, uint16_t mode, uint16_t x, uint16_t y, char character);

//! Helper function to get character from given point on screen for mode 01h on given buffer.
/*!
    Get character on given point on screen. This function works on given buffer.
    \param buffer Screen buffer.
    \param mode Mode (only when writes to external buffer).
    \param x x coordinate.
    \param y y coordinate.
    \param character Return character.
    \return 0 if success, -1 if not implemented.
*/
int8_t __mode01h_get_char_buffer(uint16_t* buffer, uint16_t mode, uint16_t x, uint16_t y, char* character);

//! Helper function to set character color on given point on screen for mode 01h on given buffer.
/*!
    Set character color on given point on screen without changing letter. This function works on given buffer.
    \param buffer Screen buffer.
    \param mode Mode (only when writes to external buffer).
    \param x x coordinate.
    \param y y coordinate.
    \param color Color.
    \return 0 if success, -1 if not implemented.
*/
int8_t __mode01h_set_color_buffer(uint16_t* buffer, uint16_t mode, uint16_t x, uint16_t y, uint8_t color);

//! Helper function to get character color from given point on screen for mode 01h on given buffer.
/*!
    Get character color on given point on screen. This function works on given buffer.
    \param buffer Screen buffer.
    \param mode Mode (only when writes to external buffer).
    \param x x coordinate.
    \param y y coordinate.
    \param color Return color.
    \return 0 if success, -1 if not implemented.
*/
int8_t __mode01h_get_color_buffer(uint16_t* buffer, uint16_t mode, uint16_t x, uint16_t y, uint8_t* color);

//! Helper function to set character and character color on given point on screen for mode 01h on given buffer.
/*!
    Set character and character color on given point on screen. This function works on given buffer.
    \param buffer Screen buffer.
    \param mode Mode (only when writes to external buffer).
    \param x x coordinate.
    \param y y coordinate.
    \param character Character.
    \param color Character color.
    \return 0 if success, -1 if not implemented.
*/
int8_t __mode01h_set_char_and_color_buffer(uint16_t* buffer, uint16_t mode, uint16_t x, uint16_t y, char character, uint8_t color);

//! Helper function to get character and character color color from given point on screen for mode 01h on given buffer.
/*!
    Get character and character color color on given point on screen. This function works on given buffer.
    \param buffer Screen buffer.
    \param mode Mode (only when writes to external buffer).
    \param x x coordinate.
    \param character Return haracter.
    \param color Return character color.
    \return 0 if success, -1 if not implemented.
*/
int8_t __mode01h_get_char_and_color_buffer(uint16_t* buffer, uint16_t mode, uint16_t x, uint16_t y, char* character, uint8_t* color);

//! Print new line.
/*!
    \param buffer Pointer to screen buffer.
    \param x Pointer to x coordinate of screen cursor.
    \param y Pointer to y coordinate of screen cursor.
*/
void __mode01h_newline(uint16_t* buffer, uint16_t* x, uint16_t* y);

//! Calculate position.
/*!
    Calculate position on screen. Function return value that added to base address will give you position of current character on screen.
    \param x x coordinate.
    \param y y coordinate.
    \return Calculated position on screen.
*/
uint16_t __mode01h_calcualte_position(uint16_t x, uint16_t y);

#endif