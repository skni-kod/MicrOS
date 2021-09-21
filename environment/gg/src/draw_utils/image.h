#ifndef IMAGE_H
#define IMAGE_H

#include "../stdafx.h"
#include "../util/rect.h"

typedef struct image
{
    int32_t width;
    int32_t height;
    byte* data;
} image;

typedef struct image_clip
{
    int32_t x;
    int32_t y;
    int32_t w;
    int32_t h;
} image_clip;

image* initImage();
image* loadImage(char* filename, bool isCompressed);

void draw(image* img, int32_t x, int32_t y, rect* camera);
void drawClipped(image* img, int32_t x, int32_t y, image_clip* clip, rect* camera);

void drawTransparent(image* img, int32_t x, int32_t y, uint8_t colorKey, rect* camera);
void drawClippedTransparent(image* img, int32_t x, int32_t y, image_clip* clip, uint8_t colorKey, rect* camera);
void drawClippedTransparentC(image* img, int32_t x, int32_t y, image_clip* clip, uint8_t colorKey, uint8_t color, rect* camera);

#endif