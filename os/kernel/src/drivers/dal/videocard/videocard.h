#ifndef VIDEOCARD_H
#define VIDEOCARD_H

#include <stdint.h>

typedef struct _videoMode
{
    uint16_t id;
    uint32_t width;
    uint32_t height;
    uint64_t colors;
    uint8_t monochrome;
    uint8_t planar;
} VideoMode;

typedef struct _driverInitStruct
{
    VideoMode* (*getAvailableGraphicVideoModes)(uint32_t*);
    VideoMode* (*getAvailableTextVideoModes)(uint32_t*);
    int16_t (*setVideoMode)(uint16_t);
    uint8_t (*isTextMode)();
    VideoMode (*getCurrentVideoMode)();
    int8_t (*turnOnBuffer)();
    int8_t (*turnOffBuffer)();
    uint8_t (*isBufferOn)();
    int8_t (*swapBuffers)();
    int8_t (*drawPixel)(uint8_t, uint16_t, uint16_t);
    int8_t (*drawLine)(uint8_t, uint16_t, uint16_t, uint16_t, uint16_t);
    int8_t (*drawCircle)(uint8_t, uint16_t, uint16_t, uint16_t);
    int8_t (*drawRectangle)(uint8_t, uint16_t, uint16_t, uint16_t, uint16_t);
    int8_t (*clearScreen)();
} DriverInitStruct;

void video_card_init_with_driver(DriverInitStruct* initStruct);

void video_card_set_get_available_graphic_video_modes_func(VideoMode* (*getAvailableGraphicVideoModes)(uint32_t*));
void video_card_set_get_available_text_video_modes_func(VideoMode* (*getAvailableTextVideoModes)(uint32_t*));
void video_card_set_set_video_mode_func(int16_t (*setVideoMode)(uint16_t));
void video_card_set_is_text_mode_func(uint8_t (*isTextMode)());
void video_card_set_get_current_video_mode_func(VideoMode (*getCurrentVideoMode)());
void video_card_set_turn_on_buffer_func(int8_t (*turnOnBuffer)());
void video_card_set_turn_off_buffer_func(int8_t (*turnOffBuffer)());
void video_card_set_is_buffer_on_func(uint8_t (*isBufferOn)());
void video_card_set_swap_buffers_func(int8_t (*swapBuffers)());
void video_card_set_draw_pixel_func(int8_t (*drawPixel)(uint8_t, uint16_t, uint16_t));
void video_card_set_draw_line_func(int8_t (*drawLine)(uint8_t, uint16_t, uint16_t, uint16_t, uint16_t));
void video_card_set_draw_circle_func(int8_t (*drawCircle)(uint8_t, uint16_t, uint16_t, uint16_t));
void video_card_set_draw_rectangle_func(int8_t (*drawRectangle)(uint8_t, uint16_t, uint16_t, uint16_t, uint16_t));
void video_card_set_clear_screen_func(int8_t (*clearScreen)());

VideoMode* getAvailableGraphicVideoModes(uint32_t *noOfModes);
VideoMode* getAvailableTextVideoModes(uint32_t *noOfModes);

int16_t video_card_set_video_mode(uint16_t mode);
uint8_t video_card_is_text_mode();
VideoMode* video_card_get_current_video_mode();

int8_t video_card_turn_on_buffer();
int8_t video_card_turn_off_buffer();
uint8_t video_card_is_buffer_on();

int8_t video_card_swap_buffers();

int8_t video_card_draw_pixel(uint8_t color, uint16_t x, uint16_t y);
int8_t video_card_draw_line(uint8_t color, uint16_t ax, uint16_t ay, uint16_t bx, uint16_t by);
int8_t video_card_draw_circle(uint8_t color, uint16_t x, uint16_t y, uint16_t radius);
int8_t video_card_draw_rectangle(uint8_t color, uint16_t ax, uint16_t ay, uint16_t bx, uint16_t by);
int8_t video_card_clear_screen();

#endif