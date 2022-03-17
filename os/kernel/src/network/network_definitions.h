/*
    @JakubPrzystasz
    Created: 06.02.2021
    Modify: 15.02.2021
*/
#ifndef network_definitions
#define network_definitions

#include <stdint.h>
#include "../klibrary/kvector.h"
#include "../klibrary/ktime.h"

#define MAC_ADDRESS_LENGTH 6
#define IPv4_ADDRESS_LENGTH 4
#define HW_TYPE_ETHERNET 0x1

#define MAX_ETH_FRAME_SIZE 1792

#define MAC_ADDRESS_SIZE sizeof(uint8_t) * MAC_ADDRESS_LENGTH
#define IPv4_ADDRESS_SIZE sizeof(uint8_t) * IPv4_ADDRESS_LENGTH

// Stucture definitions

//! Device configuration structure
/* TODO: make more options
    Device configuration structure
*/
typedef struct device_configuration
{
    //Mode 
    /* 
        bits:  MSB                           LSB
               | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
        if all bits cleared -> device is in standby mode
        if bit 0 set: receive
        if bit 1 set: send           
    */
    uint8_t mode;
} device_configuration_t;

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
    char *device_name;
    uint8_t mac_address[MAC_ADDRESS_LENGTH];
    uint8_t ipv4_address[IPv4_ADDRESS_LENGTH];
    void (*send_packet)(net_packet_t *packet);
    void (*receive_packet)(net_packet_t *packet);
    uint32_t frames_sent;
    uint32_t frames_received;
    uint32_t bytes_sent;
    uint32_t bytes_received;
    device_configuration_t *configuration;
} net_device_t;

#endif