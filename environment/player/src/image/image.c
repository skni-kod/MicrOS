#include "image.h"
#include <stdlib.h>

image* create_image(int width, int height, int bpp)
{
    image* surf = (image*)malloc(sizeof(image));
    surf->width = width;
    surf->height = height;
    surf->bpp = bpp;
    surf->data = (uint8_t*)calloc(width*height*bpp, 1);
    return surf;
}