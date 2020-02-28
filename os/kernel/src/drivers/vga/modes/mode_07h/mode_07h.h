#ifndef MODE_07H
#define MODE_07H

#include "stdint.h"

#define MODE07H_WIDTH 80
#define MODE07H_HEIGHT 25

// TODO: implement functions

int8_t mode07h_set_mode();

int8_t mode07h_turn_on_buffer();
int8_t mode07h_turn_off_buffer();
uint8_t mode07h_is_buffer_on();

int8_t mode07h_swap_buffers();

//! Print character on screen for mode 07h.
/*!
    Print character on screen without changing color of character and background.
    \param character Character.
    \return 0 if success, -1 if not implemented.
*/
int8_t mode07h_print_char(char character);

//! Print character on screen for mode 07h.
/*!
    Print character on screen with given color of character and background.
    \param character Character.
    \param color Color.
    \return 0 if success, -1 if not implemented.
*/
int8_t mode07h_print_char_color(char character, uint8_t color);

//! Print string on screen for mode 07h.
/*!
    Print string on screen without changing color of characters and background.
    \param string String.
    \return 0 if success, -1 if not implemented.
*/
int8_t mode07h_print_string(const char* string);

//! Print string on screen for mode 07h.
/*!
    Print string on screen with given color of characters and background.
    \param string String.
    \param color Color.
    \return 0 if success, -1 if not implemented.
*/
int8_t mode07h_print_string_color(const char* string, uint8_t color);

//! Set character on given point on screen for mode 07h.
/*!
    Set character on given point on screen without changing color of character and background.
    \param x x coordinate.
    \param y y coordinate.
    \param character Character.
    \return 0 if success, -1 if not implemented.
*/
int8_t mode07h_set_char(uint16_t x, uint16_t y, char character);

//! Get character from given point on screen for mode 07h.
/*!
    Get character on given point on screen.
    \param x x coordinate.
    \param y y coordinate.
    \param character Return character.
    \return 0 if success, -1 if not implemented.
*/
int8_t mode07h_get_char(uint16_t x, uint16_t y, char* character);

//! Set character color on given point on screen for mode 07h.
/*!
    Set character color on given point on screen without changing letter.
    \param x x coordinate.
    \param y y coordinate.
    \param color Color.
    \return 0 if success, -1 if not implemented.
*/
int8_t mode07h_set_color(uint16_t x, uint16_t y, uint8_t color);

//! Get character color from given point on screen for mode 07h.
/*!
    Get character color on given point on screen.
    \param x x coordinate.
    \param y y coordinate.
    \param color Return color.
    \return 0 if success, -1 if not implemented.
*/
int8_t mode07h_get_color(uint16_t x, uint16_t y, uint8_t* color);

//! Set character and character color on given point on screen for mode 07h.
/*!
    Set character and character color on given point on screen.
    \param x x coordinate.
    \param y y coordinate.
    \param character Character.
    \param color Character color.
    \return 0 if success, -1 if not implemented.
*/
int8_t mode07h_set_char_and_color(uint16_t x, uint16_t y, char character, uint8_t color);

//! Get character and character color color from given point on screen for mode 07h.
/*!
    Get character and character color color on given point on screen.
    \param x x coordinate.
    \param character Return haracter.
    \param color Return character color.
    \return 0 if success, -1 if not implemented.
*/
int8_t mode07h_get_char_and_color(uint16_t x, uint16_t y, char* character, uint8_t* color);

//! Set cursor position for mode 07h.
/*!
    Set cursor position on screen.
    \param x x coordinate.
    \param y y coordinate.
    \return 0 if success, -1 if not implemented.
*/
int8_t mode07h_set_cursor_pos(uint16_t x, uint16_t y);

//! Get cursor position for mode 07h.
/*!
    Get cursor position on screen.
    \param x Return x coordinate.
    \param y Return y coordinate.
    \return 0 if success, -1 if not implemented.
*/
int8_t mode07h_get_cursor_pos(uint16_t* x, uint16_t* y);

//! Turn on cursor for mode 07h.
/*!
    Turn on cursor with default size and position.
    Copy screen from one to another.
    \return 0 if success, -1 if not implemented.
*/
int8_t mode07h_turn_cursor_on();

//! Turn off cursor for mode 07h.
/*!
    \return 0 if success, -1 if not implemented.
*/
int8_t mode07h_turn_cursor_off();

int8_t mode07h_draw_pixel(uint8_t color, uint16_t x, uint16_t y);
int8_t mode07h_draw_line(uint8_t color, uint16_t ax, uint16_t ay, uint16_t bx, uint16_t by);
int8_t mode07h_draw_circle(uint8_t color, uint16_t x, uint16_t y, uint16_t radius);
int8_t mode07h_draw_rectangle(uint8_t color, uint16_t ax, uint16_t ay, uint16_t bx, uint16_t by);
int8_t mode07h_clear_screen();

int8_t mode07h_draw_pixel_buffered(uint8_t color, uint16_t x, uint16_t y);
int8_t mode07h_draw_line_buffered(uint8_t color, uint16_t ax, uint16_t ay, uint16_t bx, uint16_t by);
int8_t mode07h_draw_circle_buffered(uint8_t color, uint16_t x, uint16_t y, uint16_t radius);
int8_t mode07h_draw_rectangle_buffered(uint8_t color, uint16_t ax, uint16_t ay, uint16_t bx, uint16_t by);
int8_t mode07h_clear_screen_buffered();

int8_t mode07h_draw_pixel_external_buffer(uint8_t* buffer, uint16_t mode, int8_t color, uint16_t x, uint16_t y);
int8_t mode07h_draw_line_external_buffer(uint8_t* buffer, uint16_t mode, uint8_t color, uint16_t ax, uint16_t ay, uint16_t bx, uint16_t by);
int8_t mode07h_draw_circle_external_buffer(uint8_t* buffer, uint16_t mode, uint8_t color, uint16_t x, uint16_t y, uint16_t radius);
int8_t mode07h_draw_rectangle_external_buffer(uint8_t* buffer, uint16_t mode, uint8_t color, uint16_t ax, uint16_t ay, uint16_t bx, uint16_t by);
int8_t mode07h_clear_screen_external_buffer(uint8_t* buffer, uint16_t mode);

int8_t mode07h_swap_external_buffer(uint8_t* buffer, uint16_t mode);
uint8_t* mode07h_create_external_buffer(uint16_t mode);

#endif