#ifndef VGA_GMODE_H
#define VGA_GMODE_H

#include <stdint.h>

#define VGA_VRAM 0xC00A0000
#define VGA_VRAM_2 0xC00B8000
#define PITCH 320





/*void set3HVideoMode();

/*void drawMicrOSLogoIn13H();
void drawLenaIn13H();*/

/*void setModeYVideoMode();
void pixel_ModeY(unsigned char color, unsigned int x, unsigned int y);
void drawDupaInY(int color);*/


char getMode();

#endif