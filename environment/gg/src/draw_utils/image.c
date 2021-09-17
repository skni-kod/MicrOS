#include "image.h"
#include "../util/bmp.h"
#include "../util/util.h"
#include "../util/screen.h"
#include "../util/file_utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern byte* gpuBuffer;

typedef uint8_t byte;

image* initImage()
{
    image* img = NULL;
    img = (image*)calloc(1, sizeof(image));
    return img;
}

image* loadImage(char* filename, bool isCompressed)
{
    image* img = initImage();
    if(img == NULL) return NULL;

    byte* contents = NULL;
    loadFile(filename, &contents, isCompressed);

    //Now proper bmp reading starts, since we expect files to be in correct format we can just simply realloc given pointer and copy data to it.
    //In normal circumstances I'd do some checks if it is proper BMP file, but we're kinda limited in space.
    //if it fails, then I'll make another floppy just for assets.
    bmpFileHeader* fileHeader = (bmpFileHeader*)(contents);
    bmpInfoHeader* infoHeader = (bmpInfoHeader*)(contents+sizeof(bmpFileHeader));
    img->width = infoHeader->width;
    img->height = infoHeader->height;
    if(img->data != NULL) free(img->data);
    img->data = (byte*)malloc(infoHeader->width * infoHeader->height);
    int j = 0;
    uint8_t padding = (4 - infoHeader->width % 4) % 4;
    for(int i = infoHeader->height - 1; i >= 0; i--, j++)
    {
        
        int fileOffset= i * (infoHeader->width+padding);
        int arrayOffset = j * infoHeader->width;
        memcpy(((byte*)img->data)+arrayOffset, contents+fileHeader->dataOffset+fileOffset, infoHeader->width);
    }
    free(contents);
    
    return img;
}

//ALL THESE FUNCTIONS WORK IN SCREENSPACE, NOT WORLD SPACE!!!

void draw(image* img, int32_t x, int32_t y)
{
    //draw to 320x200 buffor
    //xc - counter for width of image, yc - counter for height of image
    uint32_t xc = 0, yc = 0;

    if(img != NULL)
    {
        if(x < -img->width || x >= img->width + SCREEN_WIDTH) return;
        if(y < -img->height || y >= img->height + SCREEN_HEIGHT) return;

        for(int i = y; i <= img->height + y - 1; i++, yc++)
        {
            for(int j = x; j <= img->width + x - 1; j++, xc++)    
            {
                if(j < 0 || j >= SCREEN_WIDTH) continue;
                if(i < 0 || i >= SCREEN_HEIGHT) continue;

                gpuBuffer[i * SCREEN_WIDTH + j] = img->data[yc*img->width+xc];
            }
            xc = 0;
        }
    }
}

void drawClipped(image* img, int32_t x, int32_t y, image_clip* clip)
{
    //fallback for NULL clip, otherwise it will segfault.
    if(clip == NULL) return draw(img, x,y);
    //xc - counter for width of image, yc - counter for height of image
    uint32_t xc = 0,yc = 0;

    if(img != NULL)
    {
        if(x < -clip->w || x >= clip->w + SCREEN_WIDTH) return;
        if(y < -clip->h || y >= clip->h + SCREEN_HEIGHT) return;

        for(int i = y; i <= clip->h + y - 1; i++, yc++)
        {
            for(int j = x; j <= clip->w + x - 1; j++, xc++)    
            {
                if(j < 0 || j >= SCREEN_WIDTH) continue;
                if(i < 0 || i >= SCREEN_HEIGHT) continue;
                gpuBuffer[i*SCREEN_WIDTH + j] = img->data[yc*img->width + clip->y*img->width + clip->x + xc];
            }
            xc = 0;
        }
    }
}

void drawTransparent(image* img, int32_t x, int32_t y, uint8_t colorKey)
{
    //xc - counter for width of image, yc - counter for height of image
    uint32_t xc = 0,yc = 0;

    if(img != NULL)
    {
        if(x < -img->width || x >= img->width + SCREEN_WIDTH) return;
        if(y < -img->height || y >= img->height + SCREEN_HEIGHT) return;

        for(int i = y; i <= img->height + y - 1; i++, yc++)
        {
            for(int j = x; j <= img->width + x - 1; j++, xc++)    
            {
                if(j < 0 || j >= SCREEN_WIDTH) continue;
                if(i < 0 || i >= SCREEN_HEIGHT) continue;
                if(img->data[yc*img->width+xc] == colorKey) continue;

                gpuBuffer[i*SCREEN_WIDTH + j] = img->data[yc*img->width+xc];
            }
            xc = 0;
        }
    }
}

void drawClippedTransparent(image* img, int32_t x, int32_t y, image_clip* clip, uint8_t colorKey)
{
    //fallback for NULL clip, otherwise it will segfault.
    if(clip == NULL) return drawTransparent(img, x,y, colorKey);
    
    //xc - counter for width of image, yc - counter for height of image
    uint32_t xc = 0,yc = 0;

    if(img != NULL)
    {
        if(x < -clip->w || x >= clip->w + SCREEN_WIDTH) return;
        if(y < -clip->h || y >= clip->h + SCREEN_HEIGHT) return;

        for(int i = y; i <= clip->h + y - 1; i++, yc++)
        {
            for(int j = x; j <= clip->w + x - 1; j++, xc++)    
            {
                if(j < 0 || j >= SCREEN_WIDTH) continue;
                if(i < 0 || i >= SCREEN_HEIGHT) continue;
                if(img->data[yc*img->width + clip->y*img->width + clip->x + xc] == colorKey) continue;
                gpuBuffer[i*SCREEN_WIDTH + j] = img->data[yc*img->width + clip->y*img->width + clip->x + xc];
            }
            xc = 0;
        }
    }
}