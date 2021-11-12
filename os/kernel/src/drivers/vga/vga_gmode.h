/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * THIS FILE IS MARKED TO BE REFACTORED IN FUTURE. TRY NOT TO ADD NEW THINGS TO PARTS OF OS IN THIS FILE *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef VGA_GMODE_H
#define VGA_GMODE_H

#include <stdint.h>

void drawMicrOSLogoIn13H();
void drawLenaIn13H();
void drawLenaIn10fH_linear();
void drawLenaIn10fH();

char vga_gmode_get_mode();

#endif