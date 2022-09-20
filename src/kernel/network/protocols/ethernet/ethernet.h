/*
    @JakubPrzystasz
    Created: 12.02.2021
    Modified: 17.02.2021
*/
#ifndef network_ethernet
#define network_ethernet

#include <inet/inet.h>
#include <inet/ethernet.h>
#include <inet/arp.h>
#include <network/protocols/ipv4/ipv4.h>
#include <network/network_utils.h>
#include <network/network_device.h>
#include <network/network_manager.h>


void ethernet_process_frame(nic_data_t *data);

//! network_manager_send_ethernet_frame
/*
    Send ethernet frame
*/
bool network_manager_send_ethernet_frame(ethernet_frame_t *frame, uint32_t data_len);

//! network_manager_make_frame
/*
    Make ethernet frame, with specified data and type
*/
nic_data_t *ethernet_create_frame(net_device_t *device, uint16_t type, uint32_t payload_size);

//! network_manager_send_frame
/*
    Send ethernet frame
*/
void ethernet_send_frame(nic_data_t *data, ipv4_addr_t *addr);

#endif