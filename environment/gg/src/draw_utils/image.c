#include "image.h"
#include "../util/bmp.h"
#include "../util/util.h"
#include "../util/screen.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern char partition;
extern byte* gpuBuffer;

typedef uint8_t byte;

image* initImage()
{
    image* img = NULL;
    img = (image*)calloc(1, sizeof(image));
    return img;
}

image* loadImage(char* filename)
{
    image* img = initImage();
    if(img == NULL) return NULL;

    char path[64];
    sprintf(path, "%c:%s", partition, filename);
    byte* contents;
    FILE* bmp = fopen(path, "rb");
    if(bmp == NULL)
    {
        printf("ERROR: File %s couldn't be loaded.\n", filename);
        error(-10);
    }
    int32_t fileSize;
    // obtain file size:
    fseek (bmp , 0 , SEEK_END);
    fileSize = ftell (bmp);
    rewind (bmp);
    contents = (byte*)malloc(fileSize*sizeof(byte));
    if(contents == NULL)
    {
        printf("Ooops, we're out of memory!\n");
        error(-11);
    }
    uint32_t readLen = 0;
    int32_t currentOffset = 0;
    uint32_t chunkSize = 1024;
    while(fileSize - currentOffset > 0)
    {
        if(currentOffset + 1024 > fileSize) chunkSize = fileSize - currentOffset;
        readLen += fread(contents+currentOffset, sizeof(byte), chunkSize, bmp);
        currentOffset += 1024;
    }
    if (fileSize != readLen)
    {
        printf("Reading error!\n");
        error(-12);
    }
    //we copied whole file to memory at this point, so we can safely close file.
    fclose (bmp);
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
    for(int i = infoHeader->height - 1; i >= 0; i--, j++)
    {
        int fileOffset= i * infoHeader->width;
        int arrayOffset = j * infoHeader->width;
        memcpy(((byte*)img->data)+arrayOffset, contents+fileHeader->dataOffset+fileOffset, infoHeader->width);
    }
    free(contents);
    
    return img;
}

//ALL THESE FUNCTIONS WORK IN SCREENSPACE, NOT WORLD SPACE!!!

void draw(image* img, int32_t x, int32_t y)
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

                gpuBuffer[i*SCREEN_WIDTH + j] = img->data[yc*img->width+xc];
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