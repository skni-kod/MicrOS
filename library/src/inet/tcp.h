/*
    @JakubPrzystasz
    Created: 17.02.2021
    Modified: 30.06.2022
*/
#ifndef INET_TCP_H
#define INET_TCP_H

#include <stdint.h>

typedef struct tcp_datagram
{
    uint16_t src_port;
    uint16_t dst_port;
    uint32_t seq_num;
    uint32_t ack_num;
    uint8_t reserved1 : 4;
    uint8_t offset : 4;
    uint8_t fin : 1;
    uint8_t syn : 1;
    uint8_t rst : 1;
    uint8_t psh : 1;
    uint8_t ack : 1;
    uint8_t urg : 1;
    uint8_t reserved2 : 2;
    uint16_t window;
    uint16_t checksum;
    uint16_t pointer;
    uint8_t options_data[];
} __attribute__((packed)) tcp_datagram_t;

#define TCP_OPTIONS_OFFSET (sizeof(tcp_datagram_t))

#endif