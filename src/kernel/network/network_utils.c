#include "network_utils.h"

static uint32_t crc32_table[256];

uint32_t __ip_checksum(uint8_t *data, uint32_t length, uint32_t sum)
{
    uint32_t i;

    // sum all byte pairs
    for (i = 0; i < (length & ~1U); i += 2)
    {
        sum += ntohs(*((uint16_t *)(data + i)));
        if (sum > 0xFFFF)
            sum -= 0xFFFF;
    }

    // sum leftover byte with zero padding
    if (i < length)
    {
        sum += data[i] << 8;
        if (sum > 0xFFFF)
            sum -= 0xFFFF;
    }

    return sum;
}

uint32_t __ip_wrapsum(uint32_t sum)
{
    return htons((uint16_t)(~sum & 0xFFFF));
}

// https://lxp32.github.io/docs/a-simple-example-crc32-calculation/
void __crc32_build()
{
    for (uint32_t i = 0; i < 256; i++)
    {
        uint32_t ch = i;
        uint32_t crc = 0;
        for (size_t j = 0; j < 8; j++)
        {
            uint32_t b = (ch ^ crc) & 1;
            crc >>= 1;
            if (b)
                crc = crc ^ 0xEDB88320;
            ch >>= 1;
        }
        crc32_table[i] = crc;
    }
}

uint32_t __crc32(uint8_t *data, uint32_t length)
{
    uint32_t crc = 0xFFFFFFFF;

    for (size_t i = 0; i < length; i++)
    {
        char ch = data[i];
        uint32_t t = (ch ^ crc) & 0xFF;
        crc = (crc >> 8) ^ crc32_table[t];
    }

    return ~crc;
}