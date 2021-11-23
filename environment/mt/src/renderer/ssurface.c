#include "ssurface.h"
#include <stdlib.h>

ssurface* create_surface(int width, int height, int bpp)
{
    ssurface* surf = (ssurface*)malloc(sizeof(ssurface));
    surf->width = width;
    surf->height = height;
    surf->bpp = bpp;
    surf->data = (uint8_t*)calloc(width*height*bpp, 1);
    return surf;
}