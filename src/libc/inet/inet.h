#ifndef INET_H
#define INET_H

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <micros/socket.h>
#include "ethernet.h"
#include "ipv4.h"

#define IPPROTO_IP 0
#define IPPROTO_ICMP 1
#define IPPROTO_TCP 6
#define IPPROTO_UDP 17
#define IPPROTO_RAW 255

#define INADDR_LOOPBACK 0x7F000001
#define INADDR_ANY 0x0
#define INADDR_BROADCAST 0xffffffff

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

#define HTONS(n) ((((n)&0xFF) << 8) | (((n)&0xFF00) >> 8))
#define NTOHS(n) HTONS(n)
#define HTONL(n) ((((n)&0x000000FF) << 24) | (((n)&0x0000FF00) << 8) | (((n)&0x00FF0000) >> 8) | (((n)&0xFF000000) >> 24))
#define NTOHL(n) HTONL(n)

uint16_t htons(uint16_t value);

uint16_t ntohs(uint16_t value);

uint32_t htonl(uint32_t value);

uint32_t ntohl(uint32_t value);

#define htons(n) HTONS(n)
#define ntohs(n) NTOHS(n)
#define htonl(n) HTONL(n)
#define ntohl(n) NTOHL(n)

struct sockaddr_in
{
    uint16_t sin_family;
    uint16_t sin_port;
    ipv4_addr_t sin_addr;
    uint8_t sin_zero[8];
};

ipv4_addr_t inet_addr(const char *s);

#endif
