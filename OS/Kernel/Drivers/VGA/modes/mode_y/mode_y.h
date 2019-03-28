#ifndef MODE_Y
#define MODE_Y

#include "stdint.h"

#define MODEY_WIDTH 320
#define MODEY_HEIGHT 200

int8_t setModeY();

int8_t turnOnBufferY();
int8_t turnOffBufferY();
uint8_t isBufferOnY();

int8_t swapBuffersY();

int8_t drawPixelY(uint8_t color, uint16_t x, uint16_t y);
int8_t drawLineY(uint8_t color, uint16_t ax, uint16_t ay, int16_t bx, uint16_t by);
int8_t drawCircleY(uint8_t color, uint16_t x, uint16_t y, uint16_t radius);
int8_t drawRectangleY(uint8_t color, uint16_t ax, int16_t ay, uint16_t bx, uint16_t by);
int8_t clearScreenY();

int8_t drawPixelYBuffered(uint8_t color, uint16_t x, uint16_t y);
int8_t drawLineYBuffered(uint8_t color, uint16_t ax, uint16_t ay, uint16_t bx, uint16_t by);
int8_t drawCircleYBuffered(uint8_t color, uint16_t x, uint16_t y, uint16_t radius);
int8_t drawRectangleYBuffered(uint8_t color, uint16_t ax, uint16_t ay, uint16_t bx, uint16_t by);
int8_t clearScreenYBuffered();

#endif