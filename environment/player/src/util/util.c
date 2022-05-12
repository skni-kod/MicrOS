#include "util.h"

#include <stdio.h>

uint32_t loadFile(char* filename, uint8_t** dst)
{
    FILE* in = fopen(filename, "r");
    if(in == NULL)
    {
        printf("ERROR: File %s couldn't be loaded.\n", filename);
        return 0;
    }

    uint32_t fileSize;
    // obtain file size:
    fseek (in , 0 , SEEK_END);
    fileSize = ftell (in);
    rewind (in);
    uint8_t* contents = (uint8_t*)malloc(fileSize*sizeof(uint8_t));
    if(contents == NULL)
    {
        printf("Ooops, we're out of memory!\n");
        return 0;
    }
    uint32_t readLen = 0;
    uint32_t currentOffset = 0;
    uint32_t chunkSize = 20*1024;
    while(fileSize - currentOffset > 0)
    {
        if(currentOffset + chunkSize > fileSize) chunkSize = fileSize - currentOffset;
        readLen += fread(contents+currentOffset, sizeof(uint8_t), chunkSize, in);
        currentOffset += chunkSize;
        printf("Loaded %dK bytes of file...\n", currentOffset/1024);
    }
    if (fileSize != readLen)
    {
        printf("Reading error!\n");
        return 0;
    }
    //we copied whole file to memory at this point, so we can safely close file.
    fclose (in);
    if(*dst != NULL) free(*dst);

    *dst = (uint8_t*)malloc(fileSize * sizeof(uint8_t));
    memcpy(*dst, contents, fileSize * sizeof(uint8_t));
    free(contents);
    return fileSize;
}