#include "network_utils.h"

uint8_t __uint8_flip(uint8_t value)
{
    uint8_t out;
    out = value << 4;
    out |= value >> 4;
    return out;
}

uint16_t __uint16_flip(uint16_t value)
{
    uint8_t bytes[2] = {
        (uint8_t)(*((uint8_t *)&value)),
        (uint8_t)(*((uint8_t *)&value + 1))};
    return (bytes[0] << 8) | (bytes[1]);
}

uint32_t __uint32_flip(uint32_t value)
{
    uint8_t bytes[4] = {
        (uint8_t)(*((uint8_t *)&value)),
        (uint8_t)(*((uint8_t *)&value + 1)),
        (uint8_t)(*((uint8_t *)&value + 2)),
        (uint8_t)(*((uint8_t *)&value + 3)),
    };
    return (bytes[0]) << 24 | (bytes[1] << 16) | (bytes[2] << 8) | (bytes[3]);
}

void __set_mac_addr(uint8_t *mac_addr, uint8_t oct1, uint8_t oct2, uint8_t oct3, uint8_t oct4, uint8_t oct5, uint8_t oct6)
{
    if (mac_addr == 0)
        return;

    mac_addr[0] = oct1;
    mac_addr[1] = oct2;
    mac_addr[2] = oct3;
    mac_addr[3] = oct4;
    mac_addr[4] = oct5;
    mac_addr[5] = oct6;
}

void __set_ipv4_addr(uint32_t *ip_addr, uint8_t oct1, uint8_t oct2, uint8_t oct3, uint8_t oct4)
{
    if (ip_addr == 0)
        return;

    (((uint8_t *)ip_addr)[0]) = oct1;
    (((uint8_t *)ip_addr)[1]) = oct2;
    (((uint8_t *)ip_addr)[2]) = oct3;
    (((uint8_t *)ip_addr)[3]) = oct4;
}

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
    if (data == NULL)
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
