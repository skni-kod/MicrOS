#ifndef MODE_11H
#define MODE_11H

#include "stdint.h"

#define MODE11H_WIDTH 640
#define MODE11H_HEIGHT 480

int8_t setMode11H();

int8_t turnOnBuffer11H();
int8_t turnOffBuffer11H();
uint8_t isBufferOn11H();

int8_t swapBuffers11H();

int8_t drawPixel11H(uint8_t color, uint16_t x, uint16_t y);
int8_t drawLine11H(uint8_t color, uint16_t ax, uint16_t ay, uint16_t bx, uint16_t by);
int8_t drawCircle11H(uint8_t color, uint16_t x, uint16_t y, uint16_t radius);
int8_t drawRectangle11H(uint8_t color, uint16_t ax, uint16_t ay, uint16_t bx, uint16_t by);
int8_t clearScreen11H();

int8_t drawPixel11HBuffered(uint8_t color, uint16_t x, uint16_t y);
int8_t drawLine11HBuffered(uint8_t color, uint16_t ax, uint16_t ay, uint16_t bx, uint16_t by);
int8_t drawCircle11HBuffered(uint8_t color, uint16_t x, uint16_t y, uint16_t radius);
int8_t drawRectangle11HBuffered(uint8_t color, uint16_t ax, uint16_t ay, uint16_t bx, uint16_t by);
int8_t clearScreen11HBuffered();

#endif