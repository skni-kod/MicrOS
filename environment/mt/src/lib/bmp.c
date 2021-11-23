#include "bmp.h"
#include <stdio.h>
#include <stdint.h>
#include "../renderer/srender.h"

typedef uint8_t byte;

uint32_t loadFile(char* filename, byte** dst)
{
    FILE* in = fopen(filename, "r");
    if(in == NULL)
    {
        printf("ERROR: File %s couldn't be loaded.\n", filename);
        return NULL;
    }

    int32_t fileSize;
    // obtain file size:
    fseek (in , 0 , SEEK_END);
    fileSize = ftell (in);
    rewind (in);
    byte* contents = (byte*)malloc(fileSize*sizeof(byte));
    if(contents == NULL)
    {
        printf("Ooops, we're out of memory!\n");
        return NULL;
    }
    uint32_t readLen = 0;
    int32_t currentOffset = 0;
    uint32_t chunkSize = 20*1024;
    while(fileSize - currentOffset > 0)
    {
        if(currentOffset + chunkSize > fileSize) chunkSize = fileSize - currentOffset;
        readLen += fread(contents+currentOffset, sizeof(byte), chunkSize, in);
        currentOffset += chunkSize;
    }
    if (fileSize != readLen)
    {
        printf("Reading error!\n");
        return NULL;
    }
    //we copied whole file to memory at this point, so we can safely close file.
    fclose (in);
    if(*dst != NULL) free(*dst);

    *dst = (byte*)malloc(fileSize * sizeof(byte));
    memcpy(*dst, contents, fileSize * sizeof(byte));
    free(contents);
    return fileSize;
}

ssurface* load_image(char* filename)
{
    ssurface* img = (ssurface*)malloc(sizeof(ssurface));

    byte* contents = NULL;

    loadFile(filename, &contents);
    
    bmpFileHeader* fileHeader = (bmpFileHeader*)(contents);
    bmpInfoHeader* infoHeader = (bmpInfoHeader*)(contents+sizeof(bmpFileHeader));
    
    img->width = infoHeader->width;
    img->height = infoHeader->height;
    img->bpp = 4;

    img->data = malloc(img->width*img->height*img->bpp);

    uint8_t padding = (4 - infoHeader->width % 4) % 4;
    int j = 0;
    for(int i = infoHeader->height - 1; i >= 0; i--, j++)
    {
        for(int k = 0; k < infoHeader->width; k++)
        {
            color* pix = (color*)(img->data + j * img->width * img->bpp + k * img->bpp);
            pix->b = *(contents+fileHeader->dataOffset+i*infoHeader->width*(infoHeader->bitPerPixel/8)+k*(infoHeader->bitPerPixel/8));
            pix->g = *(contents+fileHeader->dataOffset+i*infoHeader->width*(infoHeader->bitPerPixel/8)+k*(infoHeader->bitPerPixel/8)+1);
            pix->r = *(contents+fileHeader->dataOffset+i*infoHeader->width*(infoHeader->bitPerPixel/8)+k*(infoHeader->bitPerPixel/8)+2);
            pix->a = 0;
        }
        //int fileOffset= i * (infoHeader->width*(infoHeader->bitPerPixel/8)+padding);
        //int arrayOffset = j * infoHeader->width;
        //memcpy(((byte*)img->data)+arrayOffset, contents+fileHeader->dataOffset+fileOffset, infoHeader->width);
    }

    return img;
}