#ifndef VGA_GMODE_H
#define VGA_GMODE_H

#include <stdint.h>

#define VGA_VRAM 0xC00A0000
#define VGA_VRAM_2 0xC00B8000
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

void set3HVideoMode();

void set4HVideoMode();
void pixel_4H(unsigned char color, unsigned int x, unsigned int y);
void drawDupaIn4H(int color);

void set5HVideoMode();
void pixel_5H(unsigned char color, unsigned int x, unsigned int y);
void drawDupaIn5H(int color);

void set6HVideoMode();
void pixel_6H(unsigned char color, unsigned int x, unsigned int y);
void drawDupaIn6H(int color);

void setDHVideoMode();
void pixel_DH(unsigned char color, unsigned int x, unsigned int y);
void drawDupaInDH(int color);

void setEHVideoMode();
void pixel_EH(unsigned char color, unsigned int x, unsigned int y);
void drawDupaInEH(int color);

void setFHVideoMode();
void pixel_FH(unsigned char color, unsigned int x, unsigned int y);
void drawDupaInFH(int color);

void set10HVideoMode();
void pixel_10H(unsigned char color, unsigned int x, unsigned int y);
void drawDupaIn10H(int color);

void set11HVideoMode();
void pixel_11H(unsigned char color, unsigned int x, unsigned int y);
void drawDupaIn11H(int color);

void set12HVideoMode();
void pixel_12H(unsigned char color, unsigned int x, unsigned int y);
void drawDupaIn12H(int color);

void set13HVideoMode();
void pixel_13H(unsigned char color, unsigned int x, unsigned int y);
void drawDupaIn13H(int color);
void drawMicrOSLogoIn13H();
void drawLenaIn13H();

void setModeYVideoMode();
void pixel_ModeY(unsigned char color, unsigned int x, unsigned int y);
void drawDupaInY(int color);

static void set_plane(unsigned p);

void setFont(uint8_t *buf, uint8_t font_height);
char getMode();

void dumpRegs();
void printRegs();
void test13H();
void setPalette13H();

#endif