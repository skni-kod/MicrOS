/*
    @JakubPrzystasz
    Created: 17.02.2021
    Modified: 30.06.2022
*/
#ifndef INET_IPV4_H
#define INET_IPV4_H

#include <stdint.h>

#define IPv4_PROTOCOL_TYPE 0x0800
#define IPv4_PROTOCOL_VERSION 0x4

#define IPv4_FLAG_MAY_FRAGMENT 0
#define IPv4_FLAG_DONT_FRAGMENT 1
#define IPv4_FLAG_LAST_FRAGMENT 0
#define IPv4_FLAG_MORE_FRAGMENTS 1

#define IPv4_TOS_PREC_NETWORK_CONTROL 0b111
#define IPv4_TOS_PREC_INTERNETWORK_CONTROL 0b110
#define IPv4_TOS_PREC_CRITIC 0b101
#define IPv4_TOS_PREC_FLASH_OVERRIDE 0b100
#define IPv4_TOS_PREC_FLASH 0b011
#define IPv4_TOS_PREC_IMMEDIATE 0b010
#define IPv4_TOS_PREC_PRIORITY 0b001
#define IPv4_TOS_PREC_ROUTINE 0b000

typedef union ipv4_addr
{
    struct
    {
        uint8_t oct_a;
        uint8_t oct_b;
        uint8_t oct_c;
        uint8_t oct_d;
    };
    uint32_t value;
} ipv4_addr_t;

typedef enum ipv4_protocol
{
    IP_PROTOCOL_ICMP = 1,
    IP_PROTOCOL_TCP = 6,
    IP_PROTOCOL_UDP = 17,
} ipv4_protocol_t;

typedef struct ipv4_packet
{
    uint8_t ihl : 4,
        version : 4;
    union
    {
        uint8_t tos;
        struct
        {
            uint8_t : 2,
                relibility : 1,
                throughput : 1,
                delay : 1,
                precedence : 3;
        };
    };
    uint16_t length;
    uint16_t id;
    union
    {
        uint16_t flags_offset;
        struct
        {
            uint8_t offset_high : 5, : 3;
            uint8_t offset_low;
        };
        struct
        {
            uint8_t : 5,
                mf : 1,
                df : 1, : 1;
            uint8_t;
        };
        struct
        {
            uint8_t : 5,
                flags : 3;
            uint8_t;
        };
    };
    uint8_t ttl;
    uint8_t protocol;
    uint16_t header_checksum;
    ipv4_addr_t src;
    ipv4_addr_t dst;
    uint8_t data[];
} __attribute__((packed)) ipv4_packet_t;

#endif