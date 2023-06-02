#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

typedef unsigned long ULONG;

void InitCrcTable(void);
ULONG Reflect(ULONG, char);
int Get_CRC(unsigned char*, ULONG);

ULONG crc32_table[256];
ULONG ulPolynomial = 0x04c11db7;

uint32_t loadFile(char* filename, uint8_t** dst)
{
    printf("START PROG!\n");
    FILE* in = fopen(filename, "r");
    if(in == NULL)
    {
        printf("ERROR: File %s couldn't be loaded.\n", filename);
        return 0;
    }

    int32_t fileSize;
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
    printf("START LOADING!\n");
    uint32_t readLen = 0;
    uint32_t currentOffset = 0;
    uint32_t chunkSize = 100*1024;
    int32_t ogFileSize = fileSize;
    while(fileSize > 0)
    {
        uint32_t toRead = fileSize > chunkSize ? chunkSize : fileSize;
        readLen += fread(contents+currentOffset, sizeof(uint8_t), toRead, in);
        currentOffset += toRead;
        fileSize -= chunkSize;
        printf("Loaded %dK bytes of file...\n", currentOffset/1024);
    }

    if (ogFileSize != readLen)
    {
        printf("Reading error!\n");
        return 0;
    }
    //we copied whole file to memory at this point, so we can safely close file.
    fclose (in);
    if(*dst != NULL) free(*dst);

    *dst = contents;
    return ogFileSize;
}

int main(int argc, char** argv)
{
    uint8_t* data = NULL;
    uint32_t fileSize = 0;
    clock_t timeStart = clock();
    fileSize = loadFile(argv[2], &data);
    if(fileSize == 0)
    {
        printf("ERROR\n");
        return -2;
    }
    printf("LOAD DONE\n");
    clock_t endTime = clock();
    clock_t diff = endTime - timeStart;
    printf("Elapsed time %d\n", diff);

    uint8_t checksum = 0;
    for(int i = 0; i < fileSize; i++)
    {
        checksum ^= data[i];
    }

    printf("CHECK: 0x%02X\n", checksum);

    InitCrcTable();
    int crc = Get_CRC(data, fileSize);
    printf("CRC: 0x%X\n", crc);
    
    FILE* out = fopen("B:/TESTDAT.MPG", "w+");
    fwrite(data, 1, fileSize, out);
    fclose(out);
    free(data);

    fileSize = loadFile("B:/TESTDAT.MPG", &data);
    if(fileSize == 0)
    {
        printf("ERROR\n");
        return -2;
    }
    printf("LOAD DONE\n");
    endTime = clock();
    diff = endTime - timeStart;
    printf("Elapsed time %d\n", diff);

    checksum = 0;
    for(int i = 0; i < fileSize; i++)
    {
        checksum ^= data[i];
    }

    printf("CHECK: 0x%02X\n", checksum);

    InitCrcTable();
    crc = Get_CRC(data, fileSize);
    printf("CRC: 0x%X\n", crc);

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