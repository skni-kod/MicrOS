/*
    @JakubPrzystasz
    Created: 06.02.2021
    Modified: 15.02.2021
*/
#ifndef network_definitions
#define network_definitions

#include <stdint.h>
#include "../klibrary/kvector.h"
#include "../klibrary/ktime.h"

#define MAC_ADDRESS_LENGTH 6
#define IPv4_ADDRESS_LENGTH 4
#define HW_TYPE_ETHERNET 0x1
#define DEFAULT_BUFFER_SIZE (1024 * 1024 * 2)

#define MAC_ADDRESS_SIZE (sizeof(uint8_t) * MAC_ADDRESS_LENGTH)
#define IPv4_ADDRESS_SIZE (sizeof(uint8_t) * IPv4_ADDRESS_LENGTH)

#define NETWORK_MANAGER_CONFIGURATION_RX 0x1
#define NETWORK_MANAGER_CONFIGURATION_TX 0x2

typedef struct net_device net_device_t;

typedef enum net_frame_type{
    ethernet,
}net_frame_type_t;

//! nic_data
/*
    Data that comes from NIC
*/
typedef struct nic_data
{
    uint16_t length;
    net_device_t *device;
    uint8_t *frame;
    bool keep;
} nic_data_t;

typedef struct network_manager_dpi
{
    // return size of transferred data
    void (*send)(nic_data_t *data);
    void (*receive)(nic_data_t *data);
    nic_data_t *(*get_receive_buffer)(net_device_t *device, uint32_t size);
} network_manager_dpi_t;

//! Device configuration structure
/* TODO: make more options
    Device configuration structure
*/
typedef struct device_configuration
{
    // Mode
    /*
        bits:  MSB                           LSB
               | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
        if all bits cleared -> device is in standby mode
        if bit 0 set: receive
        if bit 1 set: send
    */
    uint8_t mode;
    uint8_t mac_address[MAC_ADDRESS_LENGTH];
    uint8_t ipv4_address[IPv4_ADDRESS_LENGTH];
    uint16_t mtu;
    net_frame_type_t frame_type;
    kvector *arp_entries;
} device_configuration_t;

//! net_device
/*
    Network device definition
*/
typedef struct net_device
{
    char *device_name;
    // TODO: Buffering
    //  uint8_t *receive_buffer;
    //  uint8_t *send_buffer;
    uint32_t frames_sent;
    uint32_t frames_received;
    uint32_t bytes_sent;
    uint32_t bytes_received;
    device_configuration_t *configuration;
    network_manager_dpi_t dpi;
} net_device_t;

#endif