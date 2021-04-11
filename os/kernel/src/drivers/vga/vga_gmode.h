#ifndef VGA_GMODE_H
#define VGA_GMODE_H

#include <stdint.h>

void drawMicrOSLogoIn13H();
void drawLenaIn13H();
void drawLenaIn10fH_linear(uint8_t* color);
void drawLenaIn10fH();

char vga_gmode_get_mode();

#endif