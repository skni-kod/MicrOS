#ifndef SSURFACE_H
#define SSURFACE_H

#include <stdint.h>

typedef struct _ssurface
{
    int width;
    int height;
    int bpp;
    uint8_t* data;
} ssurface;

ssurface* create_surface(int width, int height, int bpp);


#endif