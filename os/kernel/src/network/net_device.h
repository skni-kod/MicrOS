/*
    @JakubPrzystasz
    Created: 06.02.2021
*/

#ifndef net_device
#define net_device

#include "net_packet.h"

typedef struct net_device
{
    kvector *rx_queue;
    kvector *tx_queue;
    char device_name[32];
    uint8_t mac_address[6];
    void (*send_packet)(net_packet_t *packet);
    void (*receive_packet)(net_packet_t *packet);
    uint32_t (*sent_count)(void);
    uint32_t (*received_count)(void);
} net_device_t;

#endif