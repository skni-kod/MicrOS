#ifndef VIDEOCARD_H
#define VIDEOCARD_H

#include <stdint.h>

typedef struct _video_mode
{
    uint16_t id;
    uint32_t width;
    uint32_t height;
    uint64_t colors;
    uint8_t monochrome;
    uint8_t planar;
} video_mode;

typedef struct _driver_init_struct
{
    video_mode* (*get_available_graphic_video_modes)(uint32_t*);
    video_mode* (*get_available_text_video_modes)(uint32_t*);
    int16_t (*set_video_mode)(uint16_t);
    uint8_t (*is_text_mode)();
    video_mode* (*get_current_video_mode)();
    int8_t (*turn_on_buffer)();
    int8_t (*turn_off_buffer)();
    uint8_t (*is_buffer_on)();
    int8_t (*swap_buffers)();

    int8_t (*print_char)(char);
    int8_t (*print_char_color)(char, uint8_t);
    int8_t (*print_string)(const char*);
    int8_t (*print_string_color)(const char*, uint8_t);
    int8_t (*set_char)(uint16_t, uint16_t, char);
    int8_t (*get_char)(uint16_t, uint16_t, char*);
    int8_t (*set_color)(uint16_t, uint16_t, uint8_t);
    int8_t (*get_color)(uint16_t, uint16_t, uint8_t*);
    int8_t (*set_char_and_color)(uint16_t, uint16_t, char, uint8_t);
    int8_t (*get_char_and_color)(uint16_t, uint16_t, char*, uint8_t*);
    int8_t (*set_cursor_pos)(uint16_t, uint16_t);
    int8_t (*get_cursor_pos)(uint16_t*, uint16_t*);
    int8_t (*turn_cursor_on)();
    int8_t (*turn_cursor_off)();

    int8_t (*draw_pixel)(uint8_t, uint16_t, uint16_t);
    int8_t (*draw_line)(uint8_t, uint16_t, uint16_t, uint16_t, uint16_t);
    int8_t (*draw_circle)(uint8_t, uint16_t, uint16_t, uint16_t);
    int8_t (*draw_rectangle)(uint8_t, uint16_t, uint16_t, uint16_t, uint16_t);
    int8_t (*clear_screen)();

    int8_t (*print_char_external_buffer)(uint8_t*, uint16_t, uint16_t*, uint16_t*, char);
    int8_t (*print_char_color_external_buffer)(uint8_t*, uint16_t, uint16_t*, uint16_t*, char, uint8_t);
    int8_t (*print_string_external_buffer)(uint8_t*, uint16_t, uint16_t*, uint16_t*, const char*);
    int8_t (*print_string_color_external_buffer)(uint8_t*, uint16_t, uint16_t*, uint16_t*, const char*, uint8_t);
    int8_t (*set_char_external_buffer)(uint8_t*, uint16_t, uint16_t, uint16_t, char);
    int8_t (*get_char_external_buffer)(uint8_t*, uint16_t, uint16_t, uint16_t, char*);
    int8_t (*set_color_external_buffer)(uint8_t*, uint16_t, uint16_t, uint16_t, uint8_t);
    int8_t (*get_color_external_buffer)(uint8_t*, uint16_t, uint16_t, uint16_t, uint8_t*);
    int8_t (*set_char_and_color_external_buffer)(uint8_t*, uint16_t, uint16_t, uint16_t, char, uint8_t);
    int8_t (*get_char_and_color_external_buffer)(uint8_t*, uint16_t, uint16_t, uint16_t, char*, uint8_t*);

    int8_t (*draw_pixel_external_buffer)(uint8_t*, uint16_t, int8_t, uint16_t, uint16_t);
    int8_t (*draw_line_external_buffer)(uint8_t*, uint16_t, uint8_t, uint16_t, uint16_t, uint16_t, uint16_t);
    int8_t (*draw_circle_external_buffer)(uint8_t*, uint16_t, uint8_t, uint16_t, uint16_t, uint16_t);
    int8_t (*draw_rectangle_external_buffer)(uint8_t*, uint16_t, uint8_t, uint16_t, uint16_t, uint16_t, uint16_t);
    int8_t (*clear_screen_external_buffer)(uint8_t*, uint16_t, uint16_t*, uint16_t*);

    int8_t (*swap_external_buffer)(uint8_t*, uint16_t);
    uint8_t* (*create_external_buffer)(uint16_t);
    void (*destroy_external_buffer)(uint8_t*);
} driver_init_struct;

void video_card_init_with_driver(driver_init_struct* init_struct);

// Setters

void video_card_set_get_available_graphic_video_modes_func(video_mode* (*get_available_graphic_video_modes)(uint32_t*));
void video_card_set_get_available_text_video_modes_func(video_mode* (*get_available_text_video_modes)(uint32_t*));
void video_card_set_set_video_mode_func(int16_t (*set_video_mode)(uint16_t));
void video_card_set_is_text_mode_func(uint8_t (*is_text_mode)());
void video_card_set_get_current_video_mode_func(video_mode* (*get_current_video_mode)());
void video_card_set_turn_on_buffer_func(int8_t (*turn_on_buffer)());
void video_card_set_turn_off_buffer_func(int8_t (*turn_off_buffer)());
void video_card_set_is_buffer_on_func(uint8_t (*is_buffer_on)());
void video_card_set_swap_buffers_func(int8_t (*swap_buffers)());

//! Setter to set print character on screen function.
/*!
    \param print_char Pointer to appropriate function.
*/
void video_card_set_print_char_func(int8_t (*print_char)(char));

//! Setter to set print character on screen function.
/*!
    \param print_char_color Pointer to appropriate function.
*/
void video_card_set_print_char_color_func(int8_t (*print_char_color)(char, uint8_t));

//! Setter to set print string on screen function.
/*!
    \param print_string Pointer to appropriate function.
*/
void video_card_set_print_string_func(int8_t (*print_string)(const char*));

//! Setter to set print string on screen function.
/*!
    \param print_string_color Pointer to appropriate function.
*/
void video_card_set_print_string_color_func(int8_t (*print_string_color)(const char*, uint8_t));

//! Setter to set set character on given point on screen function.
/*!
    \param set_char Pointer to appropriate function.
*/
void video_card_set_set_char_func(int8_t (*set_char)(uint16_t, uint16_t, char));

//! Setter to set get character from given point on screen function.
/*!
    \param get_char Pointer to appropriate function.
*/
void video_card_set_get_char_func(int8_t (*get_char)(uint16_t, uint16_t, char*));

//! Setter to set set character color on given point on screen function.
/*!
    \param set_color Pointer to appropriate function.
*/
void video_card_set_set_color_func(int8_t (*set_color)(uint16_t, uint16_t, uint8_t));

//! Setter to set get character color from given point on screen function.
/*!
    \param get_color Pointer to appropriate function.
*/
void video_card_set_get_color_func(int8_t (*get_color)(uint16_t, uint16_t, uint8_t*));

//! Setter to set set character and character color on given point on screen function.
/*!
    \param set_char_and_color Pointer to appropriate function.
*/
void video_card_set_set_char_and_color_func(int8_t (*set_char_and_color)(uint16_t, uint16_t, char, uint8_t));

//! Setter to set get character and character color color from given point on screen function.
/*!
    \param get_char_and_color Pointer to appropriate function.
*/
void video_card_set_get_char_and_color_func(int8_t (*get_char_and_color)(uint16_t, uint16_t, char*, uint8_t*));

//! Setter to set set cursor position function.
/*!
    \param set_cursor_pos Pointer to appropriate function.
*/
void video_card_set_set_cursor_pos_func(int8_t (*set_cursor_pos)(uint16_t, uint16_t));

//! Setter to set get cursor position function.
/*!
    \param get_cursor_pos Pointer to appropriate function.
*/
void video_card_set_get_cursor_pos_func(int8_t (*get_cursor_pos)(uint16_t*, uint16_t*));

//! Setter to set turn on cursor function.
/*!
    \param turn_cursor_on Pointer to appropriate function.
*/
void video_card_set_turn_cursor_on_func(int8_t (*turn_cursor_on)());

//! Setter to set turn off cursor function.
/*!
    \param turn_cursor_off Pointer to appropriate function.
*/
void video_card_set_turn_cursor_off_func(int8_t (*turn_cursor_off)());

void video_card_set_draw_pixel_func(int8_t (*draw_pixel)(uint8_t, uint16_t, uint16_t));
void video_card_set_draw_line_func(int8_t (*draw_line)(uint8_t, uint16_t, uint16_t, uint16_t, uint16_t));
void video_card_set_draw_circle_func(int8_t (*draw_circle)(uint8_t, uint16_t, uint16_t, uint16_t));
void video_card_set_draw_rectangle_func(int8_t (*draw_rectangle)(uint8_t, uint16_t, uint16_t, uint16_t, uint16_t));
void video_card_set_clear_screen_func(int8_t (*clear_screen)());

//! Setter to set print character on screen to external buffer.
/*!
    \param print_char_external_buffer Pointer to appropriate function.
*/
void video_card_set_print_char_external_buffer(int8_t (*print_char_external_buffer)(uint8_t*, uint16_t, uint16_t*, uint16_t*, char));

//! Setter to set print character on screen to external buffer.
/*!
    \param print_char_color_external_buffer Pointer to appropriate function.
*/
void video_card_set_print_char_color_external_buffer(int8_t (*print_char_color_external_buffer)(uint8_t*, uint16_t, uint16_t*, uint16_t*, char, uint8_t));

//! Setter to set print string on screen to external buffer.
/*!
    \param print_string_external_buffer Pointer to appropriate function.
*/
void video_card_set_print_string_external_buffer(int8_t (*print_string_external_buffer)(uint8_t*, uint16_t, uint16_t*, uint16_t*, const char*));

//! Setter to set print string on screen to external buffer.
/*!
    \param print_string_color_external_buffer Pointer to appropriate function.
*/
void video_card_set_print_string_color_external_buffer(int8_t (*print_string_color_external_buffer)(uint8_t*, uint16_t, uint16_t*, uint16_t*, const char*, uint8_t));

//! Setter to set pet character on given point on screen to external buffer.
/*!
    \param set_char_external_buffer Pointer to appropriate function.
*/
void video_card_set_set_char_external_buffer(int8_t (*set_char_external_buffer)(uint8_t*, uint16_t, uint16_t, uint16_t, char));

//! Setter to set get character from given point on screen to external buffer.
/*!
    \param get_char_external_buffer Pointer to appropriate function.
*/
void video_card_set_get_char_external_buffer(int8_t (*get_char_external_buffer)(uint8_t*, uint16_t, uint16_t, uint16_t, char*));

//! Setter to set set character color on given point on screen to external buffer.
/*!
    \param set_color_external_buffer Pointer to appropriate function.
*/
void video_card_set_set_color_external_buffer(int8_t (*set_color_external_buffer)(uint8_t*, uint16_t, uint16_t, uint16_t, uint8_t));

//! Setter to set get character color from given point on screen to external buffer.
/*!
    \param get_color_external_buffer Pointer to appropriate function.
*/
void video_card_set_get_color_external_buffer(int8_t (*get_color_external_buffer)(uint8_t*, uint16_t, uint16_t, uint16_t, uint8_t*));

//! Setter to set set character and character color on given point on screen to external buffer.
/*!
    \param set_char_and_color_external_buffer Pointer to appropriate function.
*/
void video_card_set_set_char_and_color_external_buffer(int8_t (*set_char_and_color_external_buffer)(uint8_t*, uint16_t, uint16_t, uint16_t, char, uint8_t));

//! Setter to set get character and character color color from given point on screen to external buffer.
/*!
    \param get_char_and_color_external_buffer Pointer to appropriate function.
*/
void video_card_set_get_char_and_color_external_buffer(int8_t (*get_char_and_color_external_buffer)(uint8_t*, uint16_t, uint16_t, uint16_t, char*, uint8_t*));

// Graphic functions

video_mode* video_card_get_available_graphic_modes(uint32_t *noOfModes);
video_mode* video_card_get_available_text_modes(uint32_t *noOfModes);

int16_t video_card_set_video_mode(uint16_t mode);
uint8_t video_card_is_text_mode();
video_mode* video_card_get_current_video_mode();

int8_t video_card_turn_on_buffer();
int8_t video_card_turn_off_buffer();
uint8_t video_card_is_buffer_on();

int8_t video_card_swap_buffers();

//! Print character on screen.
/*!
    Print character on screen without changing color of character and background.
    \param character Character.
    \return 0 if success, -1 if not implemented.
*/
int8_t video_card_print_char(char character);

//! Print character on screen.
/*!
    Print character on screen with given color of character and background.
    \param character Character.
    \param color Color.
    \return 0 if success, -1 if not implemented.
*/
int8_t video_card_print_char_color(char character, uint8_t color);

//! Print string on screen.
/*!
    Print string on screen without changing color of characters and background.
    \param string String.
    \return 0 if success, -1 if not implemented.
*/
int8_t video_card_print_string(const char* string);

//! Print string on screen.
/*!
    Print string on screen with given color of characters and background.
    \param string String.
    \param color Color.
    \return 0 if success, -1 if not implemented.
*/
int8_t video_card_print_string_color(const char* string, uint8_t color);

//! Set character on given point on screen.
/*!
    Set character on given point on screen without changing color of character and background.
    \param x x coordinate.
    \param y y coordinate.
    \param character Character.
    \return 0 if success, -1 if not implemented.
*/
int8_t video_card_set_char(uint16_t x, uint16_t y, char character);

//! Get character from given point on screen.
/*!
    Get character on given point on screen.
    \param x x coordinate.
    \param y y coordinate.
    \param character Return character.
    \return 0 if success, -1 if not implemented.
*/
int8_t video_card_get_char(uint16_t x, uint16_t y, char* character);

//TODO add documentation
void video_card_set_swap_external_buffer(int8_t (*swap_external_buffer(uint8_t*, uint16_t)));

//! Set character color on given point on screen.
/*!
    Set character color on given point on screen without changing letter.
    \param x x coordinate.
    \param y y coordinate.
    \param color Color.
    \return 0 if success, -1 if not implemented.
*/
int8_t video_card_set_color(uint16_t x, uint16_t y, uint8_t color);

//! Get character color from given point on screen.
/*!
    Get character color on given point on screen.
    \param x x coordinate.
    \param y y coordinate.
    \param color Return color.
    \return 0 if success, -1 if not implemented.
*/
int8_t video_card_get_color(uint16_t x, uint16_t y, uint8_t* color);

//! Set character and character color on given point on screen.
/*!
    Set character and character color on given point on screen.
    \param x x coordinate.
    \param y y coordinate.
    \param character Character.
    \param color Character color.
    \return 0 if success, -1 if not implemented.
*/
int8_t video_card_set_char_and_color(uint16_t x, uint16_t y, char character, uint8_t color);

//! Get character and character color color from given point on screen.
/*!
    Get character and character color color on given point on screen.
    \param x x coordinate.
    \param character Return haracter.
    \param color Return character color.
    \return 0 if success, -1 if not implemented.
*/
int8_t video_card_get_char_and_color(uint16_t x, uint16_t y, char* character, uint8_t* color);

//! Set cursor position.
/*!
    Set cursor position on screen.
    \param x x coordinate.
    \param y y coordinate.
    \return 0 if success, -1 if not implemented.
*/
int8_t video_card_set_cursor_pos(uint16_t x, uint16_t y);

//! Get cursor position.
/*!
    Get cursor position on screen.
    \param x Return x coordinate.
    \param y Return y coordinate.
    \return 0 if success, -1 if not implemented.
*/
int8_t video_card_get_cursor_pos(uint16_t* x, uint16_t* y);

//! Turn on cursor.
/*!
    Turn on cursor with default size and position.
    Copy screen from one to another.
    \return 0 if success, -1 if not implemented.
*/
int8_t video_card_turn_cursor_on();

//! Turn off cursor.
/*!
    \return 0 if success, -1 if not implemented.
*/
int8_t video_card_turn_cursor_off();

int8_t video_card_draw_pixel(uint8_t color, uint16_t x, uint16_t y);
int8_t video_card_draw_line(uint8_t color, uint16_t ax, uint16_t ay, uint16_t bx, uint16_t by);
int8_t video_card_draw_circle(uint8_t color, uint16_t x, uint16_t y, uint16_t radius);
int8_t video_card_draw_rectangle(uint8_t color, uint16_t ax, uint16_t ay, uint16_t bx, uint16_t by);
int8_t video_card_clear_screen();

//! Print character on screen to external buffer.
/*!
    Print character on screen without changing color of character and background. This function works on external buffer.
    \param buffer Screen buffer.
    \param mode Mode.
    \param x X coordinate of screen cursor also returns new position of cursor.
    \param y Y coordinate of screen cursor also returns new position of cursor.
    \param character Character.
    \return 0 if success, -1 if not implemented.
*/
int8_t video_card_print_char_external_buffer(uint8_t* buffer, uint16_t mode, uint16_t* x, uint16_t* y, char character);

//! Print character on screen to external buffer.
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
int8_t video_card_print_char_color_external_buffer(uint8_t* buffer, uint16_t mode, uint16_t* x, uint16_t* y, char character, uint8_t color);

//! Print string on screen to external buffer.
/*!
    Print string on screen without changing color of characters and background. This function works on external buffer.
    \param buffer Screen buffer.
    \param mode Mode.
    \param x X coordinate of screen cursor also returns new position of cursor.
    \param y Y coordinate of screen cursor also returns new position of cursor.
    \param string String.
    \return 0 if success, -1 if not implemented.
*/
int8_t video_card_print_string_external_buffer(uint8_t* buffer, uint16_t mode, uint16_t* x, uint16_t* y, const char* string);

//! Print string on screen to external buffer.
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
int8_t video_card_print_string_color_external_buffer(uint8_t* buffer, uint16_t mode, uint16_t* x, uint16_t* y, const char* string, uint8_t color);

//! Set character on given point on screen to external buffer.
/*!
    Set character on given point on screen without changing color of character and background. This function works on external buffer.
    \param buffer Screen buffer.
    \param mode Mode.
    \param x x coordinate.
    \param y y coordinate.
    \param character Character.
    \return 0 if success, -1 if not implemented.
*/
int8_t video_card_set_char_external_buffer(uint8_t* buffer, uint16_t mode, uint16_t x, uint16_t y, char character);

//! Get character from given point on screen to external buffer.
/*!
    Get character on given point on screen. This function works on external buffer.
    \param buffer Screen buffer.
    \param mode Mode.
    \param x x coordinate.
    \param y y coordinate.
    \param character Return character.
    \return 0 if success, -1 if not implemented.
*/
int8_t video_card_get_char_external_buffer(uint8_t* buffer, uint16_t mode, uint16_t x, uint16_t y, char* character);

//! Set character color on given point on screen to external buffer.
/*!
    Set character color on given point on screen without changing letter. This function works on external buffer.
    \param buffer Screen buffer.
    \param mode Mode.
    \param x x coordinate.
    \param y y coordinate.
    \param color Color.
    \return 0 if success, -1 if not implemented.
*/
int8_t video_card_set_color_external_buffer(uint8_t* buffer, uint16_t mode, uint16_t x, uint16_t y, uint8_t color);

//! Get character color from given point on screen to external buffer.
/*!
    Get character color on given point on screen. This function works on external buffer.
    \param buffer Screen buffer.
    \param mode Mode.
    \param x x coordinate.
    \param y y coordinate.
    \param color Return color.
    \return 0 if success, -1 if not implemented.
*/
int8_t video_card_get_color_external_buffer(uint8_t* buffer, uint16_t mode, uint16_t x, uint16_t y, uint8_t* color);

//! Set character and character color on given point on screen to external buffer.
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
int8_t video_card_set_char_and_color_external_buffer(uint8_t* buffer, uint16_t mode, uint16_t x, uint16_t y, char character, uint8_t color);

//! Get character and character color color from given point on screen to external buffer.
/*!
    Get character and character color color on given point on screen. This function works on external buffer.
    \param buffer Screen buffer.
    \param mode Mode.
    \param x x coordinate.
    \param character Return haracter.
    \param color Return character color.
    \return 0 if success, -1 if not implemented.
*/
int8_t video_card_get_char_and_color_external_buffer(uint8_t* buffer, uint16_t mode, uint16_t x, uint16_t y, char* character, uint8_t* color);

int8_t video_card_draw_pixel_external_buffer(uint8_t* buffer, uint16_t mode, int8_t color, uint16_t x, uint16_t y);
int8_t video_card_draw_line_external_buffer(uint8_t* buffer, uint16_t mode, uint8_t color, uint16_t ax, uint16_t ay, uint16_t bx, uint16_t by);
int8_t video_card_draw_circle_external_buffer(uint8_t* buffer, uint16_t mode, uint8_t color, uint16_t x, uint16_t y, uint16_t radius);
int8_t video_card_draw_rectangle_external_buffer(uint8_t* buffer, uint16_t mode, uint8_t color, uint16_t ax, uint16_t ay, uint16_t bx, uint16_t by);
int8_t video_card_clear_screen_external_buffer(uint8_t* buffer, uint16_t mode, uint16_t* x, uint16_t* y);

int8_t video_card_swap_external_buffer(uint8_t* buffer, uint16_t mode);
uint8_t* video_card_create_external_buffer(uint16_t mode);
void video_card_destroy_external_buffer(uint8_t* buffer);

#endif