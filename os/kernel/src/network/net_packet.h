#ifndef net_packet
#define net_packet

#include <stdint.h>

typedef struct net_packet
{
    void *packet_data;
    uint32_t packet_length;
    uint8_t device_mac[6];
} net_packet_t;

#endif