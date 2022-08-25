/*
    @JakubPrzystasz
    Created: 17.02.2021
    Modified: 30.06.2022
*/
#ifndef network_udp_definitions
#define network_udp_definitions

#include "../../network_definitions.h"
#include <stdint.h>

typedef struct udp_datagram
{
    uint16_t src_port;
    uint16_t dst_port;
    uint16_t length;
    uint16_t checksum;
    uint8_t data[];
} __attribute__((packed)) udp_datagram_t;

typedef struct udp_pheader
{
    uint32_t src;
    uint32_t dst;
    uint8_t reserved;
    uint8_t protocol;
    uint16_t plength;
    uint16_t src_port;
    uint16_t dst_port;
    uint16_t length;
    uint16_t checksum;
} __attribute__((packed)) udp_pheader_t;

#endif