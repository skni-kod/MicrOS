/*
    @JakubPrzystasz
    Created: 13.02.2021
    Modified: 17.02.2021
*/
#ifndef network_utils
#define network_utils

#include <stdint.h>
#include <stdbool.h>
#include <inet/inet.h>

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