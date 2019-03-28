#ifndef MODE_0FH
#define MODE_0FH

#include "stdint.h"

#define MODE0FH_WIDTH 640
#define MODE0FH_HEIGHT 350

int8_t setMode0FH();

int8_t turnOnBuffer0FH();
int8_t turnOffBuffer0FH();
uint8_t isBufferOn0FH();

int8_t swapBuffers0FH();

int8_t drawPixel0FH(uint8_t color, uint16_t x, uint16_t y);
int8_t drawLine0FH(uint8_t color, uint16_t ax, uint16_t ay, int16_t bx, uint16_t by);
int8_t drawCircle0FH(uint8_t color, uint16_t x, uint16_t y, uint16_t radius);
int8_t drawRectangle0FH(uint8_t color, uint16_t ax, int16_t ay, uint16_t bx, uint16_t by);
int8_t clearScreen0FH();

int8_t drawPixel0FHBuffered(uint8_t color, uint16_t x, uint16_t y);
int8_t drawLine0FHBuffered(uint8_t color, uint16_t ax, uint16_t ay, uint16_t bx, uint16_t by);
int8_t drawCircle0FHBuffered(uint8_t color, uint16_t x, uint16_t y, uint16_t radius);
int8_t drawRectangle0FHBuffered(uint8_t color, uint16_t ax, uint16_t ay, uint16_t bx, uint16_t by);
int8_t clearScreen0FHBuffered();

#endif