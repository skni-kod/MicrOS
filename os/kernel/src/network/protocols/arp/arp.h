/*
    @JakubPrzystasz
    Created: 12.02.2021
    Modify: 17.02.2021
*/
#ifndef network_arp
#define network_arp
#include "arp_definitions.h"
#include "../../network_utils.h"
#include "../../network_manager.h"

//! arp_process_packet
/*
    Process incoming arp packet
*/
void arp_process_packet(nic_data_t *data);

//! arp_add_entry
/*
    Add arp entry to vector
*/
void arp_add_entry(net_device_t *device, uint8_t *mac_address, uint8_t *ip_address, arp_entry_type_t type);

//! arp_get_entry
/*
    Returns MAC address of desired IP.
    If not present in arp table, sends request
*/
arp_entry_t *arp_get_entry(net_device_t *device, uint8_t *ip_address);

//! arp_send_request
/*
    Send ARP request
*/
void arp_send_request(net_device_t *device, uint8_t *ip_address);

//! __arp_flip_values
/*
    Flip literal value
*/
void __arp_flip_values(arp_packet_t *packet);

bool __arp_compare_mac_address(uint8_t *addr1, uint8_t *addr2);
#endif
