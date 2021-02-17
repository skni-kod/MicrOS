#include "network_utils.h"

uint8_t __uint8_flip(uint8_t value)
{
    uint8_t out = 0;
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

bool __compare_mac_address(uint8_t *first, uint8_t *second)
{
    if (first == 0 || second == 0)
        return false;

    for (char i = 0; i < 6; i++)
    {
        if (*(first + i) != *(second + i))
            return false;
    }

    return true;
}

bool __compare_ip_address(uint8_t *first, uint8_t *second)
{
    if (first == 0 || second == 0)
        return false;

    for (char i = 0; i < 4; i++)
    {
        if (*(first + i) != *(second + i))
            return false;
    }

    return true;
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

void __set_ipv4_addr(uint8_t *ip_addr, uint8_t oct1, uint8_t oct2, uint8_t oct3, uint8_t oct4)
{
    if (ip_addr == 0)
        return;

    ip_addr[0] = oct1;
    ip_addr[1] = oct2;
    ip_addr[2] = oct3;
    ip_addr[3] = oct4;
}