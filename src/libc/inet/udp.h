/*
    @JakubPrzystasz
    Created: 17.02.2021
    Modified: 30.06.2022
*/
#ifndef INET_UDP_H
#define INET_UDP_H

#include <stdint.h>

typedef enum udp_port
{
    DNS          = 53,
    BOOTP_SERVER = 67,
    BOOTP_CLIENT = 68,
} udp_port_t;

typedef struct udp_datagram
{
    uint16_t src_port;
    uint16_t dst_port;
    uint16_t length;
    uint16_t checksum;
    uint8_t data[];
} __attribute__((packed)) udp_datagram_t;

#endif