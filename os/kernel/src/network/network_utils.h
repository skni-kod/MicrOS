/*
    @JakubPrzystasz
    Created: 13.02.2021
    Modified: 17.02.2021
*/
#ifndef network_utils
#define network_utils

#define ntohl(x) (__uint32_flip(x))
#define ntohs(x) (__uint16_flip(x))

#define htonl(x) (__uint32_flip(x))
#define htons(x) (__uint16_flip(x))

#include <stdint.h>
#include <stdbool.h>
#include "protocols/ethernet/ethernet_definitions.h"
#include "protocols/ipv4/ipv4_definitions.h"
#include "protocols/udp/udp_definitions.h"
#include "protocols/tcp/tcp_definitions.h"

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)       \
    (byte & 0x80 ? '1' : '0'),     \
        (byte & 0x40 ? '1' : '0'), \
        (byte & 0x20 ? '1' : '0'), \
        (byte & 0x10 ? '1' : '0'), \
        (byte & 0x08 ? '1' : '0'), \
        (byte & 0x04 ? '1' : '0'), \
        (byte & 0x02 ? '1' : '0'), \
        (byte & 0x01 ? '1' : '0')

//! __uint8_flip
/*
    Flip bits in 8 bit-wide field
*/
uint8_t __uint8_flip(uint8_t value);
//! __uint16_flip
/*
    Flip bits in 16 bit-wide field
*/
uint16_t __uint16_flip(uint16_t value);

//! __uint32_flip
/*
    Flip bits in 32 bit-wide field
*/
uint32_t __uint32_flip(uint32_t value);

//! __set_mac_addr
/*
    Sets MAC addr
*/
void __set_mac_addr(uint8_t *mac_addr, uint8_t oct1, uint8_t oct2, uint8_t oct3, uint8_t oct4, uint8_t oct5, uint8_t oct6);

//! __set_ipv4_addr
/*
    Sets IP addr
*/
void __set_ipv4_addr(uint32_t *ip_addr, uint8_t oct1, uint8_t oct2, uint8_t oct3, uint8_t oct4);

//! __ip_checksum
/*
    One's compliement checksum
*/
uint32_t __ip_checksum(unsigned char *buf, uint32_t nbytes, uint32_t sum);

//! __ip_wrapsum
/*
    One's compliement checksum
*/
uint32_t __ip_wrapsum(uint32_t sum);


//! __crc32
/*
    CRC32 checksum
*/
uint32_t __crc32(uint8_t *data, uint32_t length);
#endif