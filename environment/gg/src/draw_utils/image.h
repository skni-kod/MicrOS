#ifndef IMAGE_H
#define IMAGE_H

#include <stdint.h>

typedef uint8_t byte;

typedef struct image
{
    uint16_t width;
    uint16_t height;
    byte* data;
} image;

typedef struct image_clip
{
    uint16_t x;
    uint16_t y;
    uint16_t w;
    uint16_t h;
} image_clip;

image* initImage();
image* loadImage(char* filename);

void draw(image* img, int32_t x, int32_t y);
void drawClipped(image* img, int32_t x, int32_t y, image_clip* clip);

void drawTransparent(image* img, int32_t x, int32_t y, uint8_t colorKey);
void drawClippedTransparent(image* img, int32_t x, int32_t y, image_clip* clip, uint8_t colorKey);

#endif