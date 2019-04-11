#include "videocard.h"
#include <stddef.h>

VideoMode* (*_getAvailableGraphicVideoModes)(uint32_t*) = NULL;
VideoMode* (*_getAvailableTextVideoModes)(uint32_t*) = NULL;

int16_t (*_setVideoMode)(uint16_t) = NULL;
uint8_t (*_isTextMode)() = NULL;
VideoMode* (*_getCurrentVideoMode)() = NULL;

int8_t (*_turnOnBuffer)() = NULL;
int8_t (*_turnOffBuffer)() = NULL;
uint8_t (*_isBufferOn)() = NULL;

int8_t (*_swapBuffers)() = NULL;

int8_t (*_drawPixel)(uint8_t, uint16_t, uint16_t) = NULL;
int8_t (*_drawLine)(uint8_t, uint16_t, uint16_t, uint16_t, uint16_t) = NULL;
int8_t (*_drawCircle)(uint8_t, uint16_t, uint16_t, uint16_t) = NULL;
int8_t (*_drawRectangle)(uint8_t, uint16_t, uint16_t, uint16_t, uint16_t) = NULL;
int8_t (*_clearScreen)() = NULL;

void video_card_init_with_driver(DriverInitStruct* initStruct){
    _getAvailableGraphicVideoModes = initStruct->getAvailableGraphicVideoModes;
    _getAvailableTextVideoModes = initStruct->getAvailableTextVideoModes;
    _setVideoMode = initStruct->setVideoMode;
    _isTextMode = initStruct->isTextMode;
    _getCurrentVideoMode = initStruct->getCurrentVideoMode;
    _turnOnBuffer = initStruct->turnOnBuffer;
    _turnOffBuffer = initStruct->turnOnBuffer;
    _isBufferOn = initStruct->isBufferOn;
    _swapBuffers = initStruct->swapBuffers;
    _drawPixel = initStruct->drawPixel;
    _drawLine = initStruct->drawLine;
    _drawCircle = initStruct->drawCircle;
    _drawRectangle = initStruct->drawRectangle;
    _clearScreen = initStruct->clearScreen;
}

void video_card_set_get_available_graphic_video_modes_func(VideoMode* (*getAvailableGraphicVideoModes)(uint32_t*)){
    _getAvailableGraphicVideoModes = getAvailableGraphicVideoModes;
}

void video_card_set_get_available_text_video_modes_func(VideoMode* (*getAvailableTextVideoModes)(uint32_t*)){
    _getAvailableTextVideoModes = getAvailableTextVideoModes;
}
void video_card_set_set_video_mode_func(int16_t (*setVideoMode)(uint16_t)){
    _setVideoMode = setVideoMode;
}
void video_card_set_is_text_mode_func(uint8_t (*isTextMode)())
{
    _isTextMode = isTextMode;
}
void video_card_set_get_current_video_mode_func(VideoMode (*getCurrentVideoMode)())
{
    _getCurrentVideoMode = getCurrentVideoMode;
}
void video_card_set_turn_on_buffer_func(int8_t (*turnOnBuffer)()){
    _turnOnBuffer = turnOnBuffer;
}
void video_card_set_turn_off_buffer_func(int8_t (*turnOffBuffer)())
{
    _turnOffBuffer = turnOffBuffer;
}
void video_card_set_is_buffer_on_func(uint8_t (*isBufferOn)())
{
    _isBufferOn = isBufferOn;
}
void video_card_set_swap_buffers_func(int8_t (*swapBuffers)())
{
    _swapBuffers = swapBuffers;
}
void video_card_set_draw_pixel_func(int8_t (*drawPixel)(uint8_t, uint16_t, uint16_t))
{
    _drawPixel = drawPixel;
}
void video_card_set_draw_line_func(int8_t (*drawLine)(uint8_t, uint16_t, uint16_t, uint16_t, uint16_t))
{
    _drawLine = drawLine;
}
void video_card_set_draw_circle_func(int8_t (*drawCircle)(uint8_t, uint16_t, uint16_t, uint16_t))
{
    _drawCircle = drawCircle;
}
void video_card_set_draw_rectangle_func(int8_t (*drawRectangle)(uint8_t, uint16_t, uint16_t, uint16_t, uint16_t)){
    _drawRectangle = drawRectangle;
}
void video_card_set_clear_screen_func(int8_t (*clearScreen)()){
    _clearScreen = clearScreen;
}

VideoMode* getAvailableGraphicVideoModes(uint32_t *noOfModes){
    return (*_getAvailableGraphicVideoModes)(noOfModes);
}

VideoMode* getAvailableTextVideoModes(uint32_t *noOfModes)
{
    return (*_getAvailableTextVideoModes)(noOfModes);
}

int16_t video_card_set_video_mode(uint16_t mode)
{
    return (*_setVideoMode)(mode);
}
uint8_t video_card_is_text_mode(){
    return (*_isTextMode)();
}

VideoMode *video_card_get_current_video_mode(){
    return (*_getCurrentVideoMode)();
}

int8_t video_card_turn_on_buffer(){
    return (*_turnOnBuffer)();
}
int8_t video_card_turn_off_buffer(){
    return (*_turnOffBuffer)();
}
uint8_t video_card_is_buffer_on(){
    return (*_isBufferOn)();
}

int8_t video_card_swap_buffers(){
    return (*_swapBuffers)();
}

int8_t video_card_draw_pixel(uint8_t color, uint16_t x, uint16_t y){
    return (*_drawPixel)(color, x, y);
}
int8_t video_card_draw_line(uint8_t color, uint16_t ax, uint16_t ay, uint16_t bx, uint16_t by){
    return (*_drawLine)(color, ax, ay, bx, by);
}
int8_t video_card_draw_circle(uint8_t color, uint16_t x, uint16_t y, uint16_t radius){
    return (*_drawCircle)(color, x, y, radius);
}
int8_t video_card_draw_rectangle(uint8_t color, uint16_t ax, uint16_t ay, uint16_t bx, uint16_t by){
    return (*_drawRectangle)(color, ax, ay, bx, by);
}
int8_t video_card_clear_screen(){
    return (*_clearScreen)();
}