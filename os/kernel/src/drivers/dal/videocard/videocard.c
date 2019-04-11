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

int8_t (*_draw_pixel)(uint8_t, uint16_t, uint16_t) = NULL;
int8_t (*_draw_line)(uint8_t, uint16_t, uint16_t, uint16_t, uint16_t) = NULL;
int8_t (*_draw_circle)(uint8_t, uint16_t, uint16_t, uint16_t) = NULL;
int8_t (*_draw_rectangle)(uint8_t, uint16_t, uint16_t, uint16_t, uint16_t) = NULL;
int8_t (*_clear_screen)() = NULL;

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
    _draw_pixel = init_struct->draw_pixel;
    _draw_line = init_struct->draw_line;
    _draw_circle = init_struct->draw_circle;
    _draw_rectangle = init_struct->draw_rectangle;
    _clear_screen = init_struct->clear_screen;
}

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