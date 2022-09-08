#ifndef INET_H
#define INET_H

#include <stdint.h>

#define MAC_ADDRESS_LENGTH 6

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

#endif
