/*
    @JakubPrzystasz
    Created: 12.02.2021
    Modified: 17.02.2021
*/
#ifndef network_arp
#define network_arp

#include <inet/arp.h>
#include <inet/ipv4.h>
#include <inet/ethernet.h>
#include <network/network_utils.h>
#include <network/network_manager.h>
#include <klibrary/ktime.h>
#include <drivers/serial/serial.h>
#include <debug_helpers/library/kernel_stdio.h>
#include "../ethernet/ethernet.h"


//! arp_process_packet
/*
    Process incoming arp packet
*/
uint32_t arp_process_packet(nic_data_t *data);

//! arp_add_entry
/*
    Add arp entry to vector
*/
void arp_add_entry(net_device_t *device, mac_addr_t *mac, ipv4_addr_t *ip, arp_entry_type_t type);

//! arp_get_entry
/*
    Returns MAC address of desired IP.
    If not present in arp table, sends request
*/
arp_entry_t *arp_get_entry(net_device_t *device, ipv4_addr_t *ip);

arp_entry_t *arp_request_entry(net_device_t *device, ipv4_addr_t *ip);

//! arp_send_request
/*
    Send ARP request
*/
void arp_send_request(net_device_t *device, ipv4_addr_t *ip);

//! __arp_flip_values
/*
    Flip literal value
*/
void __arp_flip_values(arp_packet_t *packet);

#endif
