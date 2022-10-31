/*
    @JakubPrzystasz
    Created: 17.02.2021
    Modified: 30.09.2022
*/
#ifndef INET_TCP_H
#define INET_TCP_H

#include <stdint.h>

typedef enum tcp_state
{
    TCP_ESTABLISHED = 1,
    TCP_SYN_SENT,
    TCP_SYN_RECV,
    TCP_FIN_WAIT1,
    TCP_FIN_WAIT2,
    TCP_TIME_WAIT,
    TCP_CLOSE,
    TCP_CLOSE_WAIT,
    TCP_LAST_ACK,
    TCP_LISTEN,
    TCP_CLOSING,
    TCP_NEW_SYN_RECV,

    TCP_MAX_STATES /* Leave at the end! */
} tcp_state_t;

typedef union tcp_flags
{
    uint8_t value : 6, : 2;
    struct
    {
        uint8_t fin : 1;
        uint8_t syn : 1;
        uint8_t rst : 1;
        uint8_t psh : 1;
        uint8_t ack : 1;
        uint8_t urg : 1;
    };
} __attribute__((packed)) tcp_flags_t;

typedef struct tcp_segment
{
    uint16_t src_port;
    uint16_t dst_port;
    uint32_t seq_num;
    uint32_t ack_num;
    uint8_t : 4, offset : 4;
    tcp_flags_t flags;
    uint16_t window;
    uint16_t checksum;
    uint16_t pointer;
    uint8_t options_data[];
} __attribute__((packed)) tcp_segment_t;

#define TCP_DATA_PTR(segment) ((uint8_t *)segment + (((tcp_segment_t *)segment)->offset << 2))

#define TCP_DATA_SIZE(packet) (ntohs(packet->length) - (((tcp_segment_t *)(packet->data))->offset << 2) - sizeof(tcp_segment_t))

#endif