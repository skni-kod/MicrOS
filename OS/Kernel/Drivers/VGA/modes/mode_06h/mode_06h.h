#ifndef MODE_06H
#define MODE_06H

#include "stdint.h"

#define MODE06H_WIDTH 640
#define MODE06H_HEIGHT 200

int8_t setMode06H();

int8_t turnOnBuffer06H();
int8_t turnOffBuffer06H();
uint8_t isBufferOn06H();

int8_t swapBuffers06H();

int8_t drawPixel06H(uint8_t color, uint16_t x, uint16_t y);
int8_t drawLine06H(uint8_t color, uint16_t ax, uint16_t ay, int16_t bx, uint16_t by);
int8_t drawCircle06H(uint8_t color, uint16_t x, uint16_t y, uint16_t radius);
int8_t drawRectangle06H(uint8_t color, uint16_t ax, uint16_t ay, uint16_t bx, uint16_t by);
int8_t clearScreen06H();

int8_t drawPixel06HBuffered(uint8_t color, uint16_t x, uint16_t y);
int8_t drawLine06HBuffered(uint8_t color, uint16_t ax, uint16_t ay, uint16_t bx, uint16_t by);
int8_t drawCircle06HBuffered(uint8_t color, uint16_t x, uint16_t y, uint16_t radius);
int8_t drawRectangle06HBuffered(uint8_t color, uint16_t ax, uint16_t ay, uint16_t bx, uint16_t by);
int8_t clearScreen06HBuffered();

#endif