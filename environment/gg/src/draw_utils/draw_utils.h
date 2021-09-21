#ifndef DRAW_UTILS_H
#define DRAW_UTILS_H

#include "../util/rect.h"
#include "image.h"

#include "../stdafx.h"

typedef struct bitmap_font
{
    image_clip character[256];
    image* font_img;
    int16_t space;
    int16_t nl_spacing;
} bitmap_font;

void draw_rect(rect* rectangle, byte color, rect* camera);
void draw_fill_rect(rect* rectangle, byte color);

bitmap_font* load_font(char* filename, int cell_width, int cell_height);

void draw_text(char* str, int x, int y, bitmap_font* font, uint8_t color);


#endif