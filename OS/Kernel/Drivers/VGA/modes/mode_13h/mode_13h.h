#ifndef MODE_13H
#define MODE_13H

#include "stdint.h"

#define MODE13H_WIDTH 320
#define MODE13H_HEIGHT 200

int8_t setMode13H();

int8_t turnOnBuffer13H();
int8_t turnOffBuffer13H();
uint8_t isBufferOn13H();

int8_t swapBuffers13H();

int8_t drawPixel13H(uint8_t color, uint16_t x, uint16_t y);
int8_t drawLine13H(uint8_t color, uint16_t ax, uint16_t ay, uint16_t bx, uint16_t by);
int8_t drawCircle13H(uint8_t color, uint16_t x, uint16_t y, uint16_t radius);
int8_t drawRectangle13H(uint8_t color, uint16_t ax, uint16_t ay, uint16_t bx, uint16_t by);
int8_t clearScreen13H();

int8_t drawPixel13HBuffered(uint8_t color, uint16_t x, uint16_t y);
int8_t drawLine13HBuffered(uint8_t color, uint16_t ax, uint16_t ay, uint16_t bx, uint16_t by);
int8_t drawCircle13HBuffered(uint8_t color, uint16_t x, uint16_t y, uint16_t radius);
int8_t drawRectangle13HBuffered(uint8_t color, uint16_t ax, uint16_t ay, uint16_t bx, uint16_t by);
int8_t clearScreen13HBuffered();

#endif