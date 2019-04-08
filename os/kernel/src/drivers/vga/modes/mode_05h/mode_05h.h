#ifndef MODE_05H
#define MODE_05H

#include "stdint.h"

#define MODE05H_WIDTH 320
#define MODE05H_HEIGHT 200

int8_t setMode05H();

int8_t turnOnBuffer05H();
int8_t turnOffBuffer05H();
uint8_t isBufferOn05H();

int8_t swapBuffers05H();

int8_t drawPixel05H(uint8_t color, uint16_t x, uint16_t y);
int8_t drawLine05H(uint8_t color, uint16_t ax, uint16_t ay, uint16_t bx, uint16_t by);
int8_t drawCircle05H(uint8_t color, uint16_t x, uint16_t y, uint16_t radius);
int8_t drawRectangle05H(uint8_t color, uint16_t ax, uint16_t ay, uint16_t bx, uint16_t by);
int8_t clearScreen05H();

int8_t drawPixel05HBuffered(uint8_t color, uint16_t x, uint16_t y);
int8_t drawLine05HBuffered(uint8_t color, uint16_t ax, uint16_t ay, uint16_t bx, uint16_t by);
int8_t drawCircle05HBuffered(uint8_t color, uint16_t x, uint16_t y, uint16_t radius);
int8_t drawRectangle05HBuffered(uint8_t color, uint16_t ax, uint16_t ay, uint16_t bx, uint16_t by);
int8_t clearScreen05HBuffered();

#endif