#ifndef TSS_INFO_H
#define TSS_INFO_H
#pragma pack(1)

#include <stdint.h>

typedef struct tss_info
{
    uint16_t size;
    uint32_t offset;
} tss_info;

#endif