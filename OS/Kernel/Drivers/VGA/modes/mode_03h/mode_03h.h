#ifndef MODE_03H
#define MODE_03H

#include "stdint.h"

#define MODE03H_WIDTH 80
#define MODE03H_HEIGHT 25

int8_t setMode03H();

int8_t turnOnBuffer03H();
int8_t turnOffBuffer03H();
uint8_t isBufferOn03H();

int8_t swapBuffers03H();

int8_t drawPixel03H(uint8_t color, uint16_t x, uint16_t y);
int8_t drawLine03H(uint8_t color, uint16_t ax, uint16_t ay, int16_t bx, uint16_t by);
int8_t drawCircle03H(uint8_t color, uint16_t x, uint16_t y, uint16_t radius);
int8_t drawRectangle03H(uint8_t color, uint16_t ax, uint16_t ay, uint16_t bx, uint16_t by);
int8_t clearScreen03H();

int8_t drawPixel03HBuffered(uint8_t color, uint16_t x, uint16_t y);
int8_t drawLine03HBuffered(uint8_t color, uint16_t ax, uint16_t ay, uint16_t bx, uint16_t by);
int8_t drawCircle03HBuffered(uint8_t color, uint16_t x, uint16_t y, uint16_t radius);
int8_t drawRectangle03HBuffered(uint8_t color, uint16_t ax, uint16_t ay, uint16_t bx, uint16_t by);
int8_t clearScreen03HBuffered();

#endif