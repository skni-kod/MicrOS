#include "videocard.h"

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