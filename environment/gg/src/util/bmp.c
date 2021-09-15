#include "bmp.h"
#include "util.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern char partition;

typedef uint8_t byte;

void* loadBMPToByteArray(char* filename)
{
    void* array = NULL;
    char path[64];
    sprintf(path, "%c:%s", partition, filename);
    byte* contents;
    FILE* bmp = fopen(path, "rb");
    if(bmp == NULL)
    {
        printf("ERROR: File %s couldn't be loaded.\n", filename);
        error(-10);
    }
    uint32_t fileSize;
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
    uint32_t readLen = fread(contents, sizeof(byte), fileSize, bmp);
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
    printf("W: %d, H: %d\n", infoHeader->width, infoHeader->height);
    array = malloc(infoHeader->width * infoHeader->height);
    //array = realloc(array, infoHeader->width * infoHeader->height);
    int j = 0;
    for(int i = infoHeader->height - 1; i >= 0; i--, j++)
    {
        int fileOffset= i * infoHeader->width;
        int arrayOffset = j * infoHeader->width;
        memcpy(((byte*)array)+arrayOffset, contents+fileHeader->dataOffset+fileOffset, infoHeader->width);
    }
    free(contents);
    return array;
}