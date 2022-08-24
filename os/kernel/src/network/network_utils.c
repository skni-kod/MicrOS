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

void __set_ipv4_addr(uint8_t *ip_addr, uint8_t oct1, uint8_t oct2, uint8_t oct3, uint8_t oct4)
{
    if (ip_addr == 0)
        return;

    ip_addr[0] = oct1;
    ip_addr[1] = oct2;
    ip_addr[2] = oct3;
    ip_addr[3] = oct4;
}

uint16_t __checksum(uint16_t *addr, uint16_t count)
{
    register unsigned long sum = 0;
    while (count > 1)
    {
        sum += *addr++;
        count -= 2;
    }
    // if any bytes left, pad the bytes and add
    if (count > 0)
    {
        sum += ((*addr) & __uint16_flip(0xFF00));
    }
    // Fold sum to 16 bits: add carrier to result
    while (sum >> 16)
    {
        sum = (sum & 0xffff) + (sum >> 16);
    }
    // one's complement
    sum = ~sum;
    return ((unsigned short)sum);
}

uint32_t __ip_checksum(unsigned char *buf, uint32_t nbytes, uint32_t sum)
{
    unsigned int i;

    /* Checksum all the pairs of bytes first. */
    for (i = 0; i < (nbytes & ~1U); i += 2)
    {
        sum += (uint16_t)ntohs(*((uint16_t *)(buf + i)));
        if (sum > 0xFFFF)
            sum -= 0xFFFF;
    }

    /*
     * If there's a single byte left over, checksum it, too.
     * Network byte order is big-endian, so the remaining byte is
     * the high byte.
     */
    if (i < nbytes)
    {
        sum += buf[i] << 8;
        if (sum > 0xFFFF)
            sum -= 0xFFFF;
    }

    return sum;
}

uint32_t __ip_wrapsum(uint32_t sum)
{
    sum = ~sum & 0xFFFF;
    return htons((uint16_t)sum);
}