#ifndef IMAGE_H
#define IMAGE_H

#include <stdint.h>
#include <stdbool.h>

typedef uint8_t byte;

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

void draw(image* img, int32_t x, int32_t y);
void drawClipped(image* img, int32_t x, int32_t y, image_clip* clip);

void drawTransparent(image* img, int32_t x, int32_t y, uint8_t colorKey);
void drawClippedTransparent(image* img, int32_t x, int32_t y, image_clip* clip, uint8_t colorKey);

#endif