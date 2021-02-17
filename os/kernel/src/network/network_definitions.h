/*
    @JakubPrzystasz
    Created: 06.02.2021
    Modify: 15.02.2021
*/
#ifndef network_definitions
#define network_definitions

#include <stdint.h>
#include "../klibrary/kvector.h"

#define MAC_ADDRESS_LENGTH 6
#define IPv4_ADDRESS_LENGTH 4
#define HW_TYPE_ETHERNET 0x1

#define MAC_ADDRESS_SIZE sizeof(uint8_t) * MAC_ADDRESS_LENGTH
#define IPv4_ADDRESS_SIZE sizeof(uint8_t) * IPv4_ADDRESS_LENGTH

//Stucture definitions
//! net_packet
/*
    Packet that comes from NIC
*/
typedef struct net_packet
{
    void *packet_data;
    uint32_t packet_length;
    uint8_t device_mac[MAC_ADDRESS_LENGTH];
} net_packet_t;

//! ethernet_frame
/*
    Ethernet frame
*/
typedef struct ethernet_frame
{
    uint8_t dst_mac_addr[MAC_ADDRESS_LENGTH];
    uint8_t src_mac_addr[MAC_ADDRESS_LENGTH];
    uint16_t type;
    uint8_t *data;
} __attribute__((packed)) ethernet_frame_t;

//! net_device
/*
    NIC definition
*/
typedef struct net_device
{
    kvector *rx_queue;
    kvector *tx_queue;
    char *device_name;
    uint8_t mac_address[MAC_ADDRESS_LENGTH];
    uint8_t ipv4_address[IPv4_ADDRESS_LENGTH];
    void (*send_packet)(net_packet_t *packet);
    void (*receive_packet)(net_packet_t *packet);
    uint32_t (*sent_count)(void);
    uint32_t (*received_count)(void);
} net_device_t;

#endif