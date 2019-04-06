#ifndef MODE_0DH
#define MODE_0DH

#include "stdint.h"

#define MODE0DH_WIDTH 320
#define MODE0DH_HEIGHT 200

int8_t setMode0DH();

int8_t turnOnBuffer0DH();
int8_t turnOffBuffer0DH();
uint8_t isBufferOn0DH();

int8_t swapBuffers0DH();

int8_t drawPixel0DH(uint8_t color, uint16_t x, uint16_t y);
int8_t drawLine0DH(uint8_t color, uint16_t ax, uint16_t ay, uint16_t bx, uint16_t by);
int8_t drawCircle0DH(uint8_t color, uint16_t x, uint16_t y, uint16_t radius);
int8_t drawRectangle0DH(uint8_t color, uint16_t ax, uint16_t ay, uint16_t bx, uint16_t by);
int8_t clearScreen0DH();

int8_t drawPixel0DHBuffered(uint8_t color, uint16_t x, uint16_t y);
int8_t drawLine0DHBuffered(uint8_t color, uint16_t ax, uint16_t ay, uint16_t bx, uint16_t by);
int8_t drawCircle0DHBuffered(uint8_t color, uint16_t x, uint16_t y, uint16_t radius);
int8_t drawRectangle0DHBuffered(uint8_t color, uint16_t ax, uint16_t ay, uint16_t bx, uint16_t by);
int8_t clearScreen0DHBuffered();

#endif