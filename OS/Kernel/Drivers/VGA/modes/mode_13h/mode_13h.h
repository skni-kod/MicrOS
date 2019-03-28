#ifndef MODE_13H
#define MODE_13H

#include "stdint.h"

int8_t setMode13H();

int8_t turnOnBuffer13H();
int8_t turnOffBuffer13H();
uint8_t isBufferOn13H();

int8_t swapBuffers13H();

int8_t drawPixel13H(uint8_t color, int16_t x, int16_t y);
int8_t drawLine13H(uint8_t color, int16_t ax, int16_t ay, int16_t bx, int16_t by);
int8_t drawCircle13H(uint8_t color, int16_t x, int16_t y, int16_t radius);
int8_t drawRectangle13H(uint8_t color, int16_t ax, int16_t ay, int16_t bx, int16_t by);
int8_t clearScreen13H();

#endif