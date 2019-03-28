#ifndef MODE_0EH
#define MODE_0EH

#include "stdint.h"

#define MODE0EH_WIDTH 640
#define MODE0EH_HEIGHT 480

int8_t setMode0EH();

int8_t turnOnBuffer0EH();
int8_t turnOffBuffer0EH();
uint8_t isBufferOn0EH();

int8_t swapBuffers0EH();

int8_t drawPixel0EH(uint8_t color, uint16_t x, uint16_t y);
int8_t drawLine0EH(uint8_t color, uint16_t ax, uint16_t ay, int16_t bx, uint16_t by);
int8_t drawCircle0EH(uint8_t color, uint16_t x, uint16_t y, uint16_t radius);
int8_t drawRectangle0EH(uint8_t color, uint16_t ax, int16_t ay, uint16_t bx, uint16_t by);
int8_t clearScreen0EH();

int8_t drawPixel0EHBuffered(uint8_t color, uint16_t x, uint16_t y);
int8_t drawLine0EHBuffered(uint8_t color, uint16_t ax, uint16_t ay, uint16_t bx, uint16_t by);
int8_t drawCircle0EHBuffered(uint8_t color, uint16_t x, uint16_t y, uint16_t radius);
int8_t drawRectangle0EHBuffered(uint8_t color, uint16_t ax, uint16_t ay, uint16_t bx, uint16_t by);
int8_t clearScreen0EHBuffered();

#endif