/*
    @JakubPrzystasz
    Created: 06.02.2021
*/
#ifndef net_packet
#define net_packet

#include <stdint.h>

typedef struct net_packet
{
    void *packet_data;
    uint32_t packet_length;
    uint8_t device_mac[6];
} net_packet_t;

typedef struct ethernet_frame
{
    uint8_t dst_mac_addr[6];
    uint8_t src_mac_addr[6];
    uint16_t type;
    uint8_t data[];
} __attribute__((packed)) ethernet_frame_t;

#endif