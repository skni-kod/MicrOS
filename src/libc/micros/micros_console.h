#ifndef MICROS_CONSOLE_H
#define MICROS_CONSOLE_H

#include <stdint.h>
#include <stdbool.h>
#include "micros_interrupts.h"

//! Structure representing cursor position in the console
typedef struct micros_console_position
{
    //! X (horizontal) part
    uint16_t x;
    
    //! Y (vertical) part
    uint16_t y;
} micros_console_position;

//! Enum representing VGA color
typedef enum micros_console_color
{
    micros_console_color_black = 0x00,
    micros_console_color_blue = 0x01,
    micros_console_color_green = 0x02,
    micros_console_color_cyan = 0x03,
    micros_console_color_red = 0x04,
    micros_console_color_magenta = 0x05,
    micros_console_color_brown = 0x06,
    micros_console_color_light_gray = 0x07,
    micros_console_color_dark_gray = 0x08,
    micros_console_color_light_blue = 0x09,
    micros_console_color_light_green = 0x0A,
    micros_console_color_light_cyan = 0x0B,
    micros_console_color_light_red = 0x0C,
    micros_console_color_light_magenta = 0x0D,
    micros_console_color_yellow = 0x0E,
    micros_console_color_white = 0x0F
} micros_console_color;

#ifdef __cplusplus
extern "C" {
#endif

//! Prints one char
/*!
    Prints one char at the cursor position.
    \param c Char to print.
*/
void micros_console_print_char(char c);

//! Prints string
/*!
    Prints string at the cursor position (must be ended with 0).
    \param str String to print.
*/
void micros_console_print_string(const char *str);

//! Returns char from the specified cursor position.
/*!
    Returns char from the specified cursor position.
    \param position Char position.
    \return Char at the specified position
*/
char micros_console_get_char_at_position(micros_console_position *position);

//! Sets foreground color.
/*!
    Sets foreground color which will be used for all next prints.
    \param color Foreground color.
*/
void micros_console_set_foreground_color(micros_console_color color);

//! Sets background color.
/*!
    Sets background color which will be used for all next prints.
    \param color Background color.
*/
void micros_console_set_background_color(micros_console_color color);

//! Sets cursor position.
/*!
    Sets cursor position where the next print will be started.
    \param position Cursor position.
*/
void micros_console_set_cursor_position(micros_console_position *position);

//! Gets cursor position.
/*!
    Gets cursor position and stores it in the structure.
    \param position Cursor position where data will be stored.
*/
void micros_console_get_cursor_position(micros_console_position *position);

//! Clears whole console.
/*!
    Clears whole console and sets cursor position at the begin.
*/
void micros_console_clear();

//! Sets cursor visibility.
/*!
    Sets cursor visibility (true in parameters means it will blink at the current position, otherwise it will be hided).
    \param visibility Visibility flag.
*/
void micros_console_set_cursor_visibility(bool visibility);

//TODO document these
void micros_console_set_video_mode(uint8_t mode);
void micros_console_copy_from_buffer(uint8_t* buffer, uint32_t how_many);

#ifdef __cplusplus
}
#endif

#endif