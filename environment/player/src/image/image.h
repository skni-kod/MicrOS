#ifndef IMAGE_H
#define IMAGE_H

#include <stdint.h>

typedef struct _image
{
    int width;
    int height;
    int bpp;
    uint8_t* data;
} image;

image* create_image(int width, int height, int bpp);


#endif