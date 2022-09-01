/*
    @JakubPrzystasz
    Created: 17.02.2021
    Modified: 30.06.2022
*/
#ifndef network_ipv4_definitions
#define network_ipv4_definitions

#include "../../network_definitions.h"
#include <stdint.h>

#define IPv4_PROTOCOL_TYPE 0x0800
#define IPv4_PROTOCOL_VERSION 0x4
#define IPv4_ADDRESS_LENGTH 4

#define __ipv4_compare_address(addr1, addr2) (!memcmp(addr1, addr2, IPv4_ADDRESS_LENGTH))

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

typedef enum ipv4_protocol
{
    IP_PROTOCOL_ICMP = 1,
    IP_PROTOCOL_TCP = 6,
    IP_PROTOCOL_UDP = 17,
} ipv4_protocol_t;

typedef struct ipv4_packet
{
    uint8_t ihl : 4;
    uint8_t version : 4;
    uint8_t tos_reserved: 2;
    uint8_t tos_relibility: 1;
    uint8_t tos_throughput: 1;
    uint8_t tos_delay: 1;
    uint8_t tos_precedence: 3;
    uint16_t length;
    uint16_t id;
    uint8_t offset : 5;
    uint8_t flags_mf : 1;
    uint8_t flags_df : 1;
    uint8_t flags_reserved : 1;
    uint8_t offset2;
    uint8_t ttl;
    uint8_t protocol;
    uint16_t header_checksum;
    uint8_t src_ip_byte[0];
    uint32_t src_ip;
    uint8_t dst_ip_byte[0];
    uint32_t dst_ip;
    uint8_t data[];
} __attribute__((packed)) ipv4_packet_t;

#endif