#ifndef VGA_GMODE_H
#define VGA_GMODE_H

#include <stdint.h>

#define miscPort 0x3c2
#define crtcIndexPort 0x3d4
#define crtcDataPort 0x3d5
#define sequencerIndexPort 0x3c4
#define sequencerDataPort 0x3c5
#define graphicsControllerIndexPort 0x3ce
#define graphicsControllerDataPort 0x3cf
#define attributeControllerIndexPort 0x3c0
#define attributeControllerReadPort 0x3c1
#define attributeControllerWritePort 0x3c0
#define attributeControllerResetPort 0x3da

void writeRegisters(uint8_t* registers);
void set13HVideoMode();
void pixel_256 (unsigned char color, unsigned int x, unsigned int y);
void drawDupaIn13H(int color);

#endif