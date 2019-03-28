#ifndef MODE_12H
#define MODE_12H

#include "stdint.h"

#define MODE12H_WIDTH 640
#define MODE12H_HEIGHT 480

int8_t setMode12H();

int8_t turnOnBuffer12H();
int8_t turnOffBuffer12H();
uint8_t isBufferOn12H();

int8_t swapBuffers12H();

int8_t drawPixel12H(uint8_t color, uint16_t x, uint16_t y);
int8_t drawLine12H(uint8_t color, uint16_t ax, uint16_t ay, int16_t bx, uint16_t by);
int8_t drawCircle12H(uint8_t color, uint16_t x, uint16_t y, uint16_t radius);
int8_t drawRectangle12H(uint8_t color, uint16_t ax, int16_t ay, uint16_t bx, uint16_t by);
int8_t clearScreen12H();

int8_t drawPixel12HBuffered(uint8_t color, uint16_t x, uint16_t y);
int8_t drawLine12HBuffered(uint8_t color, uint16_t ax, uint16_t ay, uint16_t bx, uint16_t by);
int8_t drawCircle12HBuffered(uint8_t color, uint16_t x, uint16_t y, uint16_t radius);
int8_t drawRectangle12HBuffered(uint8_t color, uint16_t ax, uint16_t ay, uint16_t bx, uint16_t by);
int8_t clearScreen12HBuffered();

#endif