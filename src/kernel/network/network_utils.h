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