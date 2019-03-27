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

int8_t (*_drawPixel)(uint8_t, int16_t, int16_t) = NULL;
int8_t (*_drawLine)(uint8_t, int16_t, int16_t, int16_t, int16_t) = NULL;
int8_t (*_drawCircle)(uint8_t, int16_t, int16_t, int16_t) = NULL;
int8_t (*_drawRectangle)(uint8_t, int16_t, int16_t, int16_t, int16_t) = NULL;
int8_t (*_clearScreen)() = NULL;

void initWithDriver(DriverInitStruct initStruct){
    _getAvailableGraphicVideoModes = initStruct.getAvailableGraphicVideoModes;
    _getAvailableTextVideoModes = initStruct.getAvailableTextVideoModes;
    _setVideoMode = initStruct.setVideoMode;
    _isTextMode = initStruct.setVideoMode;
    _getCurrentVideoMode = initStruct.getCurrentVideoMode;
    _turnOnBuffer = initStruct.turnOnBuffer;
    _turnOffBuffer = initStruct.turnOnBuffer;
    _isBufferOn = initStruct.isBufferOn;
    _swapBuffers = initStruct.swapBuffers;
    _drawPixel = initStruct.drawPixel;
    _drawLine = initStruct.drawLine;
    _drawCircle = initStruct.drawCircle;
    _drawRectangle = initStruct.drawRectangle;
    _clearScreen = initStruct.clearScreen;
}

void setGetAvailableGraphicVideoModesFunc(VideoMode* (*getAvailableGraphicVideoModes)(uint32_t*)){
    _getAvailableGraphicVideoModes = getAvailableGraphicVideoModes;
}

void setGetAvailableTextVideoModesFunc(VideoMode* (*getAvailableTextVideoModes)(uint32_t*)){
    _getAvailableTextVideoModes = getAvailableTextVideoModes;
}
void setSetVideoModeFunc(int16_t (*setVideoMode)(uint16_t)){
    _setVideoMode = setVideoMode;
}
void setIsTextModeFunc(uint8_t (*isTextMode)())
{
    _isTextMode = isTextMode;
}
void setGetCurrentVideoModeFunc(VideoMode* (*getCurrentVideoMode)())
{
    _getCurrentVideoMode = getCurrentVideoMode;
}
void setTurnOnBufferFunc(int8_t (*turnOnBuffer)()){
    _turnOnBuffer = turnOnBuffer;
}
void setTurnOffBufferFunc(int8_t (*turnOffBuffer)())
{
    _turnOffBuffer = turnOffBuffer;
}
void setIsBufferOnFunc(uint8_t (*isBufferOn)())
{
    _isBufferOn = isBufferOn;
}
void setSwapBuffersFunc(int8_t (*swapBuffers)())
{
    _swapBuffers = swapBuffers;
}
void setDrawPixelFunc(int8_t (*drawPixel)(uint8_t, int16_t, int16_t))
{
    _drawPixel = drawPixel;
}
void setDrawLineFunc(int8_t (*drawLine)(uint8_t, int16_t, int16_t, int16_t, int16_t))
{
    _drawLine = drawLine;
}
void setDrawCircleFunc(int8_t (*drawCircle)(uint8_t, int16_t, int16_t, int16_t))
{
    _drawCircle = drawCircle;
}
void setDrawRectangleFunc(int8_t (*drawRectangle)(uint8_t, int16_t, int16_t, int16_t, int16_t)){
    _drawRectangle = drawRectangle;
}
void setClearScreenFunc(int8_t (*clearScreen)()){
    _clearScreen = clearScreen;
}

VideoMode* getAvailableGraphicVideoModes(uint32_t *noOfModes){
    return (*_getAvailableGraphicVideoModes)(noOfModes);
}

VideoMode* getAvailableTextVideoModes(uint32_t *noOfModes)
{
    return (*_getAvailableTextVideoModes)(noOfModes);
}

int16_t setVideoMode(uint16_t mode)
{
    return (*_setVideoMode)(mode);
}
uint8_t isTextMode(){
    return (*_isTextMode)();
}

VideoMode* getCurrentVideoMode(){
    return (*_getCurrentVideoMode)();
}

int8_t turnOnBuffer(){
    return (*_turnOnBuffer)();
}
int8_t turnOffBuffer(){
    return (*_turnOffBuffer)();
}
uint8_t isBufferOn(){
    return (*_isBufferOn)();
}

int8_t swapBuffers(){
    return (*_swapBuffers)();
}

int8_t drawPixel(uint8_t color, int16_t x, int16_t y){
    return (*_drawPixel)(color, x, y);
}
int8_t drawLine(uint8_t color, int16_t ax, int16_t ay, int16_t bx, int16_t by){
    return (*_drawLine)(color, ax, ay, bx, by);
}
int8_t drawCircle(uint8_t color, int16_t x, int16_t y, int16_t radius){
    return (*_drawCircle)(color, x, y, radius);
}
int8_t drawRectangle(uint8_t color, int16_t ax, int16_t ay, int16_t bx, int16_t by){
    return (*_drawRectangle)(color, ax, ay, bx, by);
}
int8_t clearScreen(){
    return (*_clearScreen)();
}