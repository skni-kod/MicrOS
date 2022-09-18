/*
    @JakubPrzystasz
    Created: 06.02.2021
    Modified: 15.02.2021
*/
#ifndef network_definitions
#define network_definitions

#include <stdint.h>
#include <inet/inet.h>
#include <inet/ipv4.h>
#include <inet/ethernet.h>
#include <klibrary/kvector.h>
#include <klibrary/ktime.h>

#define DEFAULT_BUFFER_SIZE (1024 * 1024 * 2)


typedef struct net_device net_device_t;

typedef union net_mode
{
    uint8_t value;
    struct
    {
        uint8_t send : 1;
        uint8_t receive : 1;
    };

} net_mode_t;

//! nic_data
/*
    Data that comes from NIC
*/
typedef struct nic_data
{
    net_device_t *device;
    uint16_t length;
    uint8_t *frame;
    bool keep;
} nic_data_t;

typedef struct net_dpi
{
    uint32_t (*send)(nic_data_t *data);
    uint32_t (*receive)(nic_data_t *data);
    nic_data_t *(*get_receive_buffer)(net_device_t *device, uint32_t size);
    nic_data_t *(*get_send_buffer)(net_device_t *device, uint32_t size);
} net_dpi_t;

//! Device configuration structure
/*
    Device configuration structure
*/
typedef struct net_interface_t
{
    net_mode_t mode;
    mac_addr_t mac;
    ipv4_addr_t ipv4;
    ipv4_addr_t ipv4_gateway;
    uint16_t mtu;
    kvector *arp_entries;
} net_interface_t;

//! net_device
/*
    Network device definition
*/
typedef struct net_device
{
    char *device_name;
    uint32_t frames_sent;
    uint32_t frames_received;
    uint32_t bytes_sent;
    uint32_t bytes_received;
    net_interface_t *interface;
    net_dpi_t dpi;
} net_device_t;

#endif