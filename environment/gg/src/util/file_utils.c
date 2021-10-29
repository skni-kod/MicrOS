#include "file_utils.h"

#include <stdio.h>
#include "lz77.h"
#include <string.h>

extern char partition;

uint32_t loadFile(char* filename, byte** dst, bool compressed)
{
    char path[64];
    //SPRINTF IS FUCKED
    //sprintf(path, "%c:%s", partition, filename);
    
    *path = partition;
    *(path+1) = ':';
    int cc = 0;
    do
    {
        *(path+cc+2) = *(filename+cc);
    } while(*(filename+cc++) != NULL);
    
    FILE* in = fopen(path, "r");
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
    uint32_t chunkSize = 10*1024;
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
    //Decompression part
    if(compressed)
    {
        fileSize = decompress(contents, fileSize, dst);
    }
    else
    {
        if(*dst != NULL) free(*dst);

        *dst = (byte*)malloc(fileSize * sizeof(byte));
        memcpy(*dst, contents, fileSize * sizeof(byte));
    }
    free(contents);
    return fileSize;
}

uint32_t saveFile(char* filename, byte* src, size_t src_size, bool useCompression)
{
    char path[64];
    //SPRINTF IS FUCKED
    //sprintf(path, "%c:%s", partition, filename);
    
    *path = partition;
    *(path+1) = ':';
    int cc = 0;
    do
    {
        *(path+cc+2) = *(filename+cc);
    } while(*(filename+cc++) != NULL);

    byte* dst = NULL;
    uint32_t size = 0;
    if(useCompression)
    {
        size = compress(src, src_size, &dst, src_size, 4);
    }
    else
    {
        dst = src;
        size = src_size;
    }
    FILE* out = fopen(path, "w");

    uint32_t s = fwrite(dst, 1, size, out);

    fclose(out);

    return s;
}