/*
    @JakubPrzystasz
    Created: 27.06.2022
*/
#ifndef network_ethernet_definitions
#define network_ethernet_definitions

#define ETHERNET_FCS_LENGTH 4

#include "../../network_definitions.h"
#include <stdint.h>

//! ethernet_frame
/*
    Ethernet frame
*/
typedef struct ethernet_frame
{
    uint8_t dst_mac_addr[MAC_ADDRESS_LENGTH];
    uint8_t src_mac_addr[MAC_ADDRESS_LENGTH];
    uint16_t type;
    uint8_t data[];
} __attribute__((packed)) ethernet_frame_t;

#endif