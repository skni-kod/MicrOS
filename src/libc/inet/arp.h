/*
    @JakubPrzystasz
    Created: 13.02.2021
    Modified: 17.02.2021
*/
#ifndef INET_ARP_H
#define INET_ARP_H

#include <stdint.h>
#include "ipv4.h"
#include "ethernet.h"

#define ARP_PROTOCOL_TYPE 0x0806

#define ARP_OPCODE_REQUEST 0x1
#define ARP_OPCODE_REPLY 0x2

#define ARP_PR_TYPE 0x0800

#define ARP_HW_TYPE_ETHERNET 0x1

#define ARP_TIMEOUT 50
#define ARP_RETRY_INTERVAL 10

typedef enum arp_entry_type
{
    ARP_ENTRY_TYPE_STATIC,
    ARP_ENTRY_TYPE_DYNAMIC
} arp_entry_type_t;

typedef struct arp_packet
{
    uint16_t hardware_type;
    uint16_t protocol_type;
    uint8_t hardware_length;
    uint8_t protocol_length;
    uint16_t opcode;
    mac_addr_t src_hw;
    ipv4_addr_t src_pr;
    mac_addr_t dst_hw;
    ipv4_addr_t dst_pr;
} __attribute__((packed)) arp_packet_t;

typedef struct arp_entry
{
    mac_addr_t mac;
    ipv4_addr_t ip;
    uint32_t add_time;
    arp_entry_type_t type;
} arp_entry_t;

#endif
