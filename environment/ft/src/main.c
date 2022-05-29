#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef unsigned long ULONG;

void InitCrcTable(void);
ULONG Reflect(ULONG, char);
int Get_CRC(unsigned char*, ULONG);

ULONG crc32_table[256];
ULONG ulPolynomial = 0x04c11db7;

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

int main(int argc, char** argv)
{
    uint8_t* data = NULL;
    uint32_t fileSize = 0;
    fileSize = loadFile(argv[2], &data);
    if(fileSize == 0)
    {
        printf("ERROR\n");
        return -2;
    }
    printf("LOAD DONE\n");
    InitCrcTable();
    int crc = Get_CRC(data, fileSize);
    printf("CRC: 0x%X\n", crc);
    free(data);
    return 0;
}

void InitCrcTable()
{
    for(int i = 0; i <= 0xFF; i++)
    {
        crc32_table[i]=Reflect(i, 8) << 24;
        for (int j = 0; j < 8; j++)
            crc32_table[i] = (crc32_table[i] << 1) ^ (crc32_table[i] & (1 << 31) ? ulPolynomial : 0);
        crc32_table[i] = Reflect(crc32_table[i], 32);
    }

}

ULONG Reflect(ULONG ref, char ch)
{                                 
    ULONG value = 0;
    for(int i = 1; i < (ch + 1); i++)
    {
        if(ref & 1)
            value |= 1 << (ch - i);
        ref >>= 1;
    }
    return value;
}

int Get_CRC(unsigned char* buffer, ULONG bufsize)
{

    ULONG  crc = 0xffffffff;
    int len;
    len = bufsize;
    for(int i = 0; i < len; i++)
          crc = (crc >> 8) ^ crc32_table[(crc & 0xFF) ^ buffer[i]];

    return crc^0xffffffff;
}