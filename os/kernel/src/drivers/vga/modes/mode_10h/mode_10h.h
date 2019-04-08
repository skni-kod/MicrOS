#ifndef MODE_10H
#define MODE_10H

#include "stdint.h"

#define MODE10H_WIDTH 640
#define MODE10H_HEIGHT 480

int8_t setMode10H();

int8_t turnOnBuffer10H();
int8_t turnOffBuffer10H();
uint8_t isBufferOn10H();

int8_t swapBuffers10H();

int8_t drawPixel10H(uint8_t color, uint16_t x, uint16_t y);
int8_t drawLine10H(uint8_t color, uint16_t ax, uint16_t ay, uint16_t bx, uint16_t by);
int8_t drawCircle10H(uint8_t color, uint16_t x, uint16_t y, uint16_t radius);
int8_t drawRectangle10H(uint8_t color, uint16_t ax, uint16_t ay, uint16_t bx, uint16_t by);
int8_t clearScreen10H();

int8_t drawPixel10HBuffered(uint8_t color, uint16_t x, uint16_t y);
int8_t drawLine10HBuffered(uint8_t color, uint16_t ax, uint16_t ay, uint16_t bx, uint16_t by);
int8_t drawCircle10HBuffered(uint8_t color, uint16_t x, uint16_t y, uint16_t radius);
int8_t drawRectangle10HBuffered(uint8_t color, uint16_t ax, uint16_t ay, uint16_t bx, uint16_t by);
int8_t clearScreen10HBuffered();

#endif