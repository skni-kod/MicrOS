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
} __attribute__((packed)) VideoMode;

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
} __attribute__((packed)) DriverInitStruct;

void initWithDriver(DriverInitStruct* initStruct);

void setGetAvailableGraphicVideoModesFunc(VideoMode* (*getAvailableGraphicVideoModes)(uint32_t*));
void setGetAvailableTextVideoModesFunc(VideoMode* (*getAvailableTextVideoModes)(uint32_t*));
void setSetVideoModeFunc(int16_t (*setVideoMode)(uint16_t));
void setIsTextModeFunc(uint8_t (*isTextMode)());
void setGetCurrentVideoModeFunc(VideoMode (*getCurrentVideoMode)());
void setTurnOnBufferFunc(int8_t (*turnOnBuffer)());
void setTurnOffBufferFunc(int8_t (*turnOffBuffer)());
void setIsBufferOnFunc(uint8_t (*isBufferOn)());
void setSwapBuffersFunc(int8_t (*swapBuffers)());
void setDrawPixelFunc(int8_t (*drawPixel)(uint8_t, uint16_t, uint16_t));
void setDrawLineFunc(int8_t (*drawLine)(uint8_t, uint16_t, uint16_t, uint16_t, uint16_t));
void setDrawCircleFunc(int8_t (*drawCircle)(uint8_t, uint16_t, uint16_t, uint16_t));
void setDrawRectangleFunc(int8_t (*drawRectangle)(uint8_t, uint16_t, uint16_t, uint16_t, uint16_t));
void setClearScreenFunc(int8_t (*clearScreen)());

VideoMode* getAvailableGraphicVideoModes(uint32_t *noOfModes);
VideoMode* getAvailableTextVideoModes(uint32_t *noOfModes);


int16_t setVideoMode(uint16_t mode);
uint8_t isTextMode();
VideoMode* getCurrentVideoMode();

int8_t turnOnBuffer();
int8_t turnOffBuffer();
uint8_t isBufferOn();

int8_t swapBuffers();

int8_t drawPixel(uint8_t color, uint16_t x, uint16_t y);
int8_t drawLine(uint8_t color, uint16_t ax, uint16_t ay, uint16_t bx, uint16_t by);
int8_t drawCircle(uint8_t color, uint16_t x, uint16_t y, uint16_t radius);
int8_t drawRectangle(uint8_t color, uint16_t ax, uint16_t ay, uint16_t bx, uint16_t by);
int8_t clearScreen();




#endif