#include "videocard.h"
#include <stddef.h>

video_mode* (*_get_available_graphic_video_modes)(uint32_t*) = NULL;
video_mode* (*_get_available_text_video_modes)(uint32_t*) = NULL;

int16_t (*_set_video_mode)(uint16_t) = NULL;
uint8_t (*_is_text_mode)() = NULL;
video_mode* (*_get_current_video_mode)() = NULL;

int8_t (*_turn_on_buffer)() = NULL;
int8_t (*_turn_off_buffer)() = NULL;
uint8_t (*_is_buffer_on)() = NULL;

int8_t (*_swap_buffers)() = NULL;

int8_t (*_print_char)(char) = NULL;
int8_t (*_print_char_color)(char, uint8_t) = NULL;
int8_t (*_print_string)(const char*) = NULL;
int8_t (*_print_string_color)(const char*, uint8_t) = NULL;
int8_t (*_set_char)(uint16_t, uint16_t, char) = NULL;
int8_t (*_get_char)(uint16_t, uint16_t, char*) = NULL;
int8_t (*_set_color)(uint16_t, uint16_t, uint8_t) = NULL;
int8_t (*_get_color)(uint16_t, uint16_t, uint8_t*) = NULL;
int8_t (*_set_char_and_color)(uint16_t, uint16_t, char, uint8_t) = NULL;
int8_t (*_get_char_and_color)(uint16_t, uint16_t, char*, uint8_t*) = NULL;
int8_t (*_set_cursor_pos)(uint16_t, uint16_t) = NULL;
int8_t (*_get_cursor_pos)(uint16_t*, uint16_t*) = NULL;
int8_t (*_turn_cursor_on)() = NULL;
int8_t (*_turn_cursor_off)() = NULL;

int8_t (*_draw_pixel)(uint8_t, uint16_t, uint16_t) = NULL;
int8_t (*_draw_line)(uint8_t, uint16_t, uint16_t, uint16_t, uint16_t) = NULL;
int8_t (*_draw_circle)(uint8_t, uint16_t, uint16_t, uint16_t) = NULL;
int8_t (*_draw_rectangle)(uint8_t, uint16_t, uint16_t, uint16_t, uint16_t) = NULL;
int8_t (*_clear_screen)() = NULL;

int8_t (*_print_char_external_buffer)(uint8_t*, uint16_t, uint16_t*, uint16_t*, char) = NULL;
int8_t (*_print_char_color_external_buffer)(uint8_t*, uint16_t, uint16_t*, uint16_t*, char, uint8_t) = NULL;
int8_t (*_print_string_external_buffer)(uint8_t*, uint16_t, uint16_t*, uint16_t*, const char*) = NULL;
int8_t (*_print_string_color_external_buffer)(uint8_t*, uint16_t, uint16_t*, uint16_t*, const char*, uint8_t) = NULL;
int8_t (*_set_char_external_buffer)(uint8_t*, uint16_t, uint16_t, uint16_t, char) = NULL;
int8_t (*_get_char_external_buffer)(uint8_t*, uint16_t, uint16_t, uint16_t, char*) = NULL;
int8_t (*_set_color_external_buffer)(uint8_t*, uint16_t, uint16_t, uint16_t, uint8_t) = NULL;
int8_t (*_get_color_external_buffer)(uint8_t*, uint16_t, uint16_t, uint16_t, uint8_t*) = NULL;
int8_t (*_set_char_and_color_external_buffer)(uint8_t*, uint16_t, uint16_t, uint16_t, char, uint8_t) = NULL;
int8_t (*_get_char_and_color_external_buffer)(uint8_t*, uint16_t, uint16_t, uint16_t, char*, uint8_t*) = NULL;

int8_t (*_draw_pixel_external_buffer)(uint8_t*, uint16_t, int8_t, uint16_t, uint16_t) = NULL;
int8_t (*_draw_line_external_buffer)(uint8_t*, uint16_t, uint8_t, uint16_t, uint16_t, uint16_t, uint16_t) = NULL;
int8_t (*_draw_circle_external_buffer)(uint8_t*, uint16_t, uint8_t, uint16_t, uint16_t, uint16_t) = NULL;
int8_t (*_draw_rectangle_external_buffer)(uint8_t*, uint16_t, uint8_t, uint16_t, uint16_t, uint16_t, uint16_t) = NULL;
int8_t (*_clear_screen_external_buffer)(uint8_t*, uint16_t, uint16_t*, uint16_t*) = NULL;

int8_t (*_swap_external_buffer)(uint8_t*, uint16_t) = NULL;
uint8_t* (*_create_external_buffer)(uint16_t) = NULL;
void (*_destroy_external_buffer)(uint8_t*) = NULL;

void video_card_init_with_driver(driver_init_struct* init_struct){
    _get_available_graphic_video_modes = init_struct->get_available_graphic_video_modes;
    _get_available_text_video_modes = init_struct->get_available_text_video_modes;
    _set_video_mode = init_struct->set_video_mode;
    _is_text_mode = init_struct->is_text_mode;
    _get_current_video_mode = init_struct->get_current_video_mode;
    _turn_on_buffer = init_struct->turn_on_buffer;
    _turn_off_buffer = init_struct->turn_off_buffer;
    _is_buffer_on = init_struct->is_buffer_on;
    _swap_buffers = init_struct->swap_buffers;
    _print_char = init_struct->print_char;
    _print_char_color = init_struct->print_char_color;
    _print_string = init_struct->print_string;
    _print_string_color = init_struct->print_string_color;
    _set_char = init_struct->set_char;
    _get_char = init_struct->get_char;
    _set_color = init_struct->set_color;
    _get_color = init_struct->get_color;
    _set_char_and_color = init_struct->set_char_and_color;
    _get_char_and_color = init_struct->get_char_and_color;
    _set_cursor_pos = init_struct->set_cursor_pos;
    _get_cursor_pos = init_struct->get_cursor_pos;
    _turn_cursor_on = init_struct->turn_cursor_on;
    _turn_cursor_off = init_struct->turn_cursor_off;
    _draw_pixel = init_struct->draw_pixel;
    _draw_line = init_struct->draw_line;
    _draw_circle = init_struct->draw_circle;
    _draw_rectangle = init_struct->draw_rectangle;
    _clear_screen = init_struct->clear_screen;

    _print_char_external_buffer = init_struct->print_char_external_buffer;
    _print_char_color_external_buffer = init_struct->print_char_color_external_buffer;
    _print_string_external_buffer = init_struct->print_string_external_buffer;
    _print_string_color_external_buffer = init_struct->print_string_color_external_buffer;
    _set_char_external_buffer = init_struct->set_char_external_buffer;
    _get_char_external_buffer = init_struct->get_char_external_buffer;
    _set_color_external_buffer = init_struct->set_color_external_buffer;
    _get_color_external_buffer = init_struct->get_color_external_buffer;
    _set_char_and_color_external_buffer = init_struct->set_char_and_color_external_buffer;
    _get_char_and_color_external_buffer = init_struct->get_char_and_color_external_buffer;
    _draw_pixel_external_buffer = init_struct->draw_pixel_external_buffer;
    _draw_line_external_buffer = init_struct->draw_line_external_buffer;
    _draw_circle_external_buffer = init_struct->draw_circle_external_buffer;
    _draw_rectangle_external_buffer = init_struct->draw_rectangle_external_buffer;
    _clear_screen_external_buffer = init_struct->clear_screen_external_buffer;
    _swap_external_buffer = init_struct->swap_external_buffer;
    _create_external_buffer = init_struct->create_external_buffer;
    _destroy_external_buffer = init_struct->destroy_external_buffer;

}

// Setters

void video_card_set_get_available_graphic_video_modes_func(video_mode* (*get_available_graphic_video_modes)(uint32_t*))
{
    _get_available_graphic_video_modes = get_available_graphic_video_modes;
}

void video_card_set_get_available_text_video_modes_func(video_mode* (*get_available_text_video_modes)(uint32_t*))
{
    _get_available_text_video_modes = get_available_text_video_modes;
}

void video_card_set_set_video_mode_func(int16_t (*set_video_mode)(uint16_t))
{
    _set_video_mode = set_video_mode;
}

void video_card_set_is_text_mode_func(uint8_t (*is_text_mode)())
{
    _is_text_mode = is_text_mode;
}

void video_card_set_get_current_video_mode_func(video_mode* (*get_current_video_mode)())
{
    _get_current_video_mode = get_current_video_mode;
}

void video_card_set_turn_on_buffer_func(int8_t (*turn_on_buffer)())
{
    _turn_on_buffer = turn_on_buffer;
}

void video_card_set_turn_off_buffer_func(int8_t (*turn_off_buffer)())
{
    _turn_off_buffer = turn_off_buffer;
}

void video_card_set_is_buffer_on_func(uint8_t (*is_buffer_on)())
{
    _is_buffer_on = is_buffer_on;
}

void video_card_set_swap_buffers_func(int8_t (*swap_buffers)())
{
    _swap_buffers = swap_buffers;
}

void video_card_set_print_char_func(int8_t (*print_char)(char))
{
    _print_char = print_char;
}

void video_card_set_print_char_color_func(int8_t (*print_char_color)(char, uint8_t))
{
    _print_char_color = print_char_color;
}

void video_card_set_print_string_func(int8_t (*print_string)(const char*))
{
    _print_string = print_string;
}

void video_card_set_print_string_color_func(int8_t (*print_string_color)(const char*, uint8_t))
{
    _print_string_color = print_string_color;
}

void video_card_set_set_char_func(int8_t (*set_char)(uint16_t, uint16_t, char))
{
    _set_char = set_char;
}

void video_card_set_get_char_func(int8_t (*get_char)(uint16_t, uint16_t, char*))
{
    _get_char = get_char;
}

void video_card_set_set_color_func(int8_t (*set_color)(uint16_t, uint16_t, uint8_t))
{
    _set_color = set_color;
}

void video_card_set_get_color_func(int8_t (*get_color)(uint16_t, uint16_t, uint8_t*))
{
    _get_color = get_color;
}

void video_card_set_set_char_and_color_func(int8_t (*set_char_and_color)(uint16_t, uint16_t, char, uint8_t))
{
    _set_char_and_color = set_char_and_color;
}

void video_card_set_get_char_and_color_func(int8_t (*get_char_and_color)(uint16_t, uint16_t, char*, uint8_t*))
{
    _get_char_and_color = get_char_and_color;
}

void video_card_set_set_cursor_pos_func(int8_t (*set_cursor_pos)(uint16_t, uint16_t))
{
    _set_cursor_pos = set_cursor_pos;
}

void video_card_set_get_cursor_pos_func(int8_t (*get_cursor_pos)(uint16_t*, uint16_t*))
{
    _get_cursor_pos = get_cursor_pos;
}

void video_card_set_turn_cursor_on_func(int8_t (*turn_cursor_on)())
{
    _turn_cursor_on = turn_cursor_on;
}

void video_card_set_turn_cursor_off_func(int8_t (*turn_cursor_off)())
{
    _turn_cursor_off = turn_cursor_off;
}

void video_card_set_draw_pixel_func(int8_t (*draw_pixel)(uint8_t, uint16_t, uint16_t))
{
    _draw_pixel = draw_pixel;
}

void video_card_set_draw_line_func(int8_t (*draw_line)(uint8_t, uint16_t, uint16_t, uint16_t, uint16_t))
{
    _draw_line = draw_line;
}

void video_card_set_draw_circle_func(int8_t (*draw_circle)(uint8_t, uint16_t, uint16_t, uint16_t))
{
    _draw_circle = draw_circle;
}

void video_card_set_draw_rectangle_func(int8_t (*draw_rectangle)(uint8_t, uint16_t, uint16_t, uint16_t, uint16_t))
{
    _draw_rectangle = draw_rectangle;
}

void video_card_set_clear_screen_func(int8_t (*clear_screen)())
{
    _clear_screen = clear_screen;
}

void video_card_set_print_char_external_buffer(int8_t (*print_char_external_buffer)(uint8_t*, uint16_t, uint16_t*, uint16_t*, char))
{
    _print_char_external_buffer = print_char_external_buffer;
}

void video_card_set_print_char_color_external_buffer(int8_t (*print_char_color_external_buffer)(uint8_t*, uint16_t, uint16_t*, uint16_t*, char, uint8_t))
{
    _print_char_color_external_buffer = print_char_color_external_buffer;
}

void video_card_set_print_string_external_buffer(int8_t (*print_string_external_buffer)(uint8_t*, uint16_t, uint16_t*, uint16_t*, const char*))
{
    _print_string_external_buffer = print_string_external_buffer;
}

void video_card_set_print_string_color_external_buffer(int8_t (*print_string_color_external_buffer)(uint8_t*, uint16_t, uint16_t*, uint16_t*, const char*, uint8_t))
{
    _print_string_color_external_buffer = print_string_color_external_buffer;
}

void video_card_set_set_char_external_buffer(int8_t (*set_char_external_buffer)(uint8_t*, uint16_t, uint16_t, uint16_t, char))
{
    _set_char_external_buffer = set_char_external_buffer;
}

void video_card_set_get_char_external_buffer(int8_t (*get_char_external_buffer)(uint8_t*, uint16_t, uint16_t, uint16_t, char*))
{
    _get_char_external_buffer = get_char_external_buffer;
}

void video_card_set_set_color_external_buffer(int8_t (*set_color_external_buffer)(uint8_t*, uint16_t, uint16_t, uint16_t, uint8_t))
{
    _set_color_external_buffer = set_color_external_buffer;
}

void video_card_set_get_color_external_buffer(int8_t (*get_color_external_buffer)(uint8_t*, uint16_t, uint16_t, uint16_t, uint8_t*))
{
    _get_color_external_buffer = get_color_external_buffer;
}

void video_card_set_set_char_and_color_external_buffer(int8_t (*set_char_and_color_external_buffer)(uint8_t*, uint16_t, uint16_t, uint16_t, char, uint8_t))
{
    _set_char_and_color_external_buffer = set_char_and_color_external_buffer;
}

void video_card_set_get_char_and_color_external_buffer(int8_t (*get_char_and_color_external_buffer)(uint8_t*, uint16_t, uint16_t, uint16_t, char*, uint8_t*))
{
    _get_char_and_color_external_buffer = get_char_and_color_external_buffer;
}

// Graphic functions

video_mode* video_card_get_available_graphic_modes(uint32_t *noOfModes)
{
    return (*_get_available_graphic_video_modes)(noOfModes);
}

video_mode* video_card_get_available_text_modes(uint32_t *noOfModes)
{
    return (*_get_available_text_video_modes)(noOfModes);
}

int16_t video_card_set_video_mode(uint16_t mode)
{
    return (*_set_video_mode)(mode);
}
uint8_t video_card_is_text_mode()
{
    return (*_is_text_mode)();
}

video_mode *video_card_get_current_video_mode()
{
    return (*_get_current_video_mode)();
}

int8_t video_card_turn_on_buffer(){
    return (*_turn_on_buffer)();
}
int8_t video_card_turn_off_buffer(){
    return (*_turn_off_buffer)();
}
uint8_t video_card_is_buffer_on(){
    return (*_is_buffer_on)();
}

int8_t video_card_swap_buffers(){
    return (*_swap_buffers)();
}

int8_t video_card_print_char(char character)
{
    return (*_print_char)(character);
}
int8_t video_card_print_char_color(char character, uint8_t color)
{
    return (*_print_char_color)(character, color);
}
int8_t video_card_print_string(const char* string)
{
    return (*_print_string)(string);
}
int8_t video_card_print_string_color(const char* string, uint8_t color)
{
    return (*_print_string_color)(string, color);
}
int8_t video_card_set_char(uint16_t x, uint16_t y, char character)
{
    return (*_set_char)(x, y, character);
}
int8_t video_card_get_char(uint16_t x, uint16_t y, char* character)
{
    return (*_get_char)(x, y, character);
}
int8_t video_card_set_color(uint16_t x, uint16_t y, uint8_t color)
{
    return (*_set_color)(x, y, color);
}
int8_t video_card_get_color(uint16_t x, uint16_t y, uint8_t* color)
{
    return (*_get_color)(x, y, color);
}
int8_t video_card_set_char_and_color(uint16_t x, uint16_t y, char character, uint8_t color)
{
    return (*_set_char_and_color)(x, y, character, color);
}
int8_t video_card_get_char_and_color(uint16_t x, uint16_t y, char* character, uint8_t* color)
{
    return (*_get_char_and_color)(x, y, character, color);
}
int8_t video_card_set_cursor_pos(uint16_t x, uint16_t y)
{
    return (*_set_cursor_pos)(x, y);
}
int8_t video_card_get_cursor_pos(uint16_t* x, uint16_t* y)
{
    return (*_get_cursor_pos)(x, y);
}
int8_t video_card_turn_cursor_on()
{
    return (*_turn_cursor_on)();
}
int8_t video_card_turn_cursor_off()
{
    return (*_turn_cursor_off)();
}

int8_t video_card_draw_pixel(uint8_t color, uint16_t x, uint16_t y){
    return (*_draw_pixel)(color, x, y);
}
int8_t video_card_draw_line(uint8_t color, uint16_t ax, uint16_t ay, uint16_t bx, uint16_t by){
    return (*_draw_line)(color, ax, ay, bx, by);
}
int8_t video_card_draw_circle(uint8_t color, uint16_t x, uint16_t y, uint16_t radius){
    return (*_draw_circle)(color, x, y, radius);
}
int8_t video_card_draw_rectangle(uint8_t color, uint16_t ax, uint16_t ay, uint16_t bx, uint16_t by){
    return (*_draw_rectangle)(color, ax, ay, bx, by);
}
int8_t video_card_clear_screen(){
    return (*_clear_screen)();
}

int8_t video_card_print_char_external_buffer(uint8_t* buffer, uint16_t mode, uint16_t* x, uint16_t* y, char character)
{
    return (*_print_char_external_buffer)(buffer, mode, x, y, character);
}
int8_t video_card_print_char_color_external_buffer(uint8_t* buffer, uint16_t mode, uint16_t* x, uint16_t* y, char character, uint8_t color)
{
    return (*_print_char_color_external_buffer)(buffer, mode, x, y, character, color);
}
int8_t video_card_print_string_external_buffer(uint8_t* buffer, uint16_t mode, uint16_t* x, uint16_t* y, const char* string)
{
    return (*_print_string_external_buffer)(buffer, mode, x, y, string);
}
int8_t video_card_print_string_color_external_buffer(uint8_t* buffer, uint16_t mode, uint16_t* x, uint16_t* y, const char* string, uint8_t color)
{
    return (*_print_string_color_external_buffer)(buffer, mode, x, y, string, color);
}
int8_t video_card_set_char_external_buffer(uint8_t* buffer, uint16_t mode, uint16_t x, uint16_t y, char character)
{
    return (*_set_char_external_buffer)(buffer, mode, x, y, character);
}
int8_t video_card_get_char_external_buffer(uint8_t* buffer, uint16_t mode, uint16_t x, uint16_t y, char* character)
{
    return (*_get_char_external_buffer)(buffer, mode, x, y, character);
}
int8_t video_card_set_color_external_buffer(uint8_t* buffer, uint16_t mode, uint16_t x, uint16_t y, uint8_t color)
{
    return (*_set_color_external_buffer)(buffer, mode, x, y, color);
}
int8_t video_card_get_color_external_buffer(uint8_t* buffer, uint16_t mode, uint16_t x, uint16_t y, uint8_t* color)
{
    return (*_get_color_external_buffer)(buffer, mode, x, y, color);
}

int8_t video_card_set_char_and_color_external_buffer(uint8_t* buffer, uint16_t mode, uint16_t x, uint16_t y, char character, uint8_t color)
{
    return (*_set_char_and_color_external_buffer)(buffer, mode, x, y, character, color);
}
int8_t video_card_get_char_and_color_external_buffer(uint8_t* buffer, uint16_t mode, uint16_t x, uint16_t y, char* character, uint8_t* color)
{
    return (*_get_char_and_color_external_buffer)(buffer, mode, x, y, character, color);
}

int8_t video_card_draw_pixel_external_buffer(uint8_t* buffer, uint16_t mode, int8_t color, uint16_t x, uint16_t y){
    return (*_draw_pixel_external_buffer)(buffer, mode, color, x, y);
}
int8_t video_card_draw_line_external_buffer(uint8_t* buffer, uint16_t mode, uint8_t color, uint16_t ax, uint16_t ay, uint16_t bx, uint16_t by){
    return (*_draw_line_external_buffer)(buffer, mode, color, ax, ay, bx, by);
}
int8_t video_card_draw_circle_external_buffer(uint8_t* buffer, uint16_t mode, uint8_t color, uint16_t x, uint16_t y, uint16_t radius){
    return (*_draw_circle_external_buffer)(buffer, mode, color, x, y, radius);
}
int8_t video_card_draw_rectangle_external_buffer(uint8_t* buffer, uint16_t mode, uint8_t color, uint16_t ax, uint16_t ay, uint16_t bx, uint16_t by){
    return (*_draw_rectangle_external_buffer)(buffer, mode, color, ax, ay, bx, by);
}
int8_t video_card_clear_screen_external_buffer(uint8_t* buffer, uint16_t mode, uint16_t* x, uint16_t* y){
    return (*_clear_screen_external_buffer)(buffer, mode, x, y);
}

int8_t video_card_swap_external_buffer(uint8_t* buffer, uint16_t mode){
    return (*_swap_external_buffer)(buffer, mode);
}
uint8_t* video_card_create_external_buffer(uint16_t mode){
    return (*_create_external_buffer)(mode);
}
void video_card_destroy_external_buffer(uint8_t* buffer){
    return (*_destroy_external_buffer)(buffer);
}

void video_card_set_swap_external_buffer(int8_t (*swap_external_buffer(uint8_t*, uint16_t)))
{
    _swap_external_buffer = swap_external_buffer;
}