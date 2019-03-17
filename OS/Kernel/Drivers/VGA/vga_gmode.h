#ifndef VGA_GMODE_H
#define VGA_GMODE_H

#include <stdint.h>

#define VGA_VRAM 0xC00A0000
#define VGA_VRAM_2 0xC00B0000
#define PITCH 320

#define miscPort 0x3c2
#define crtcIndexPort 0x3d4
#define crtcDataPort 0x3d5
#define crtcIndexPortText 0x3b4
#define crtcDataPortText 0x3b5
#define sequencerIndexPort 0x3c4
#define sequencerDataPort 0x3c5
#define graphicsControllerIndexPort 0x3ce
#define graphicsControllerDataPort 0x3cf
#define attributeControllerIndexPort 0x3c0
#define attributeControllerReadPort 0x3c1
#define attributeControllerWritePort 0x3c0
#define attributeControllerResetPort 0x3da

void writeRegisters(uint8_t *registers);
void writeRegistersText(uint8_t *registers);
void set13HVideoMode();
void set3HVideoMode();
void set11HVideoMode();
void set12HVideoMode();
void setModeXVideoMode();
void pixel_13H(unsigned char color, unsigned int x, unsigned int y);
void pixel_11H(unsigned char color, unsigned int x, unsigned int y);
void pixel_12H(unsigned char color, unsigned int x, unsigned int y);
void drawDupaIn13H(int color);
void drawDupaIn11H(int color);
void drawDupaIn12H(int color);
void pixel_ModeX(unsigned char color, unsigned int x, unsigned int y);
void drawDupaInX(int color);

static void set_plane(unsigned p);

void setFont(uint8_t *buf, uint8_t font_height);
char getMode();

#endif