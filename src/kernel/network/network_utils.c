#include "network_utils.h"

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

uint32_t __crc32(uint8_t *data, uint32_t length)
{
    // https://lxp32.github.io/docs/a-simple-example-crc32-calculation/
    if (data == 0)
        return 0;

    uint32_t crc = 0xFFFFFFFF;

    while (length--)
    {
        crc ^= *data++;
        for (unsigned k = 0; k < 8; k++)
            crc = crc & 1 ? (crc >> 1) ^ 0xEDB88320 : crc >> 1;
    }

    return crc ^ 0xFFFFFFFF;
}
