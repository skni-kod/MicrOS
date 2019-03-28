#ifndef MODE_04H
#define MODE_04H

#include "stdint.h"

#define MODE04H_WIDTH 320
#define MODE04H_HEIGHT 200

int8_t setMode04H();

int8_t turnOnBuffer04H();
int8_t turnOffBuffer04H();
uint8_t isBufferOn04H();

int8_t swapBuffers04H();

int8_t drawPixel04H(uint8_t color, uint16_t x, uint16_t y);
int8_t drawLine04H(uint8_t color, uint16_t ax, uint16_t ay, int16_t bx, uint16_t by);
int8_t drawCircle04H(uint8_t color, uint16_t x, uint16_t y, uint16_t radius);
int8_t drawRectangle04H(uint8_t color, uint16_t ax, uint16_t ay, uint16_t bx, uint16_t by);
int8_t clearScreen04H();

int8_t drawPixel04HBuffered(uint8_t color, uint16_t x, uint16_t y);
int8_t drawLine04HBuffered(uint8_t color, uint16_t ax, uint16_t ay, uint16_t bx, uint16_t by);
int8_t drawCircle04HBuffered(uint8_t color, uint16_t x, uint16_t y, uint16_t radius);
int8_t drawRectangle04HBuffered(uint8_t color, uint16_t ax, uint16_t ay, uint16_t bx, uint16_t by);
int8_t clearScreen04HBuffered();

#endif