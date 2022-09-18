/*
    @JakubPrzystasz
    Created: 27.06.2022
*/
#ifndef INET_ETHERNET_H
#define INET_ETHERNET_H

#define ETHERNET_FCS_LENGTH 4
#define ETHERNET_ADDRESS_LENGTH 6


#include <stdint.h>
#include "inet.h"

typedef union mac_addr
{
    uint8_t addr[ETHERNET_ADDRESS_LENGTH];
    struct
    {
        uint8_t octet_a;
        uint8_t octet_b;
        uint8_t octet_c;
        uint8_t octet_d;
        uint8_t octet_e;
        uint8_t octet_f;
    };
} mac_addr_t;


//! ethernet_frame
/*
    Ethernet frame
*/
typedef struct ethernet_frame
{
    mac_addr_t dst;
    mac_addr_t src;
    uint16_t type;
    uint8_t data[];
} __attribute__((packed)) ethernet_frame_t;

#endif