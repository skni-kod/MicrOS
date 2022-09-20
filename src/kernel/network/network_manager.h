/*
    @JakubPrzystasz
    Created: 06.02.2021
*/
#ifndef network_manager
#define network_manager

//TODO: This sould not be here:
#include <drivers/nic/rtl8139/rtl8139.h>
#include <drivers/nic/virtio-pci-nic/virtio-pci-nic.h>

#include "protocols/ipv4/ipv4.h"

#include <logger/logger.h>
#include "network_device.h"
#include "network_utils.h"


//! network_manager_init
/*
    Initialize network manager
*/
bool network_manager_init();

//! network_manager_send_data
/*
    Send data
*/
void network_manager_send_data(nic_data_t *data);

//! network_manager_receive_data
/*
    Function triggers when data incomes to system
*/
void network_manager_receive_data(nic_data_t *data);

//! network_manager_process_data
/*
    Process incoming data
*/
void network_manager_process_data(nic_data_t *data);

//! network_manager_bytes_sent
/*
    Returns number of bytes sent from system
*/
uint64_t network_manager_bytes_sent(void);

//! network_manager_bytes_received
/*
    Returns number of bytes received to system
*/
uint64_t network_manager_bytes_received(void);

//! network_manager_get_nic_by_ipv4
/*
    Return network device according to destination IP address
*/
net_device_t *network_manager_get_nic_by_ipv4(ipv4_addr_t *addr);

//! network_manager_get_nic
/*
    Return main NIC
*/
net_device_t *network_manager_get_nic();

nic_data_t *network_manager_get_receive_buffer(net_device_t *device);

nic_data_t *network_manager_get_transmitt_buffer(net_device_t *device);

//! __network_manager_print_device_info
/*
    Print device info
*/
void __network_manager_print_device_info(net_device_t *device);

int8_t __network_manager_set_net_device(net_device_t *device);
#endif