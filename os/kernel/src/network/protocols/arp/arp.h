/*
    @JakubPrzystasz
    Created: 12.02.2021
*/
#ifndef arp_protocol
#define arp_protocol
#include "arp_definitions.h"
#include "../../network_utils.h"
#include "../../network_manager.h"

//! arp_process_packet
/*
    Process incoming arp packet
*/
void arp_process_packet(arp_packet_t *packet, uint8_t *device_mac);

// !arp_add_entry
/*
    Add arp entry to vector
*/
void arp_add_entry(uint8_t *mac_address, uint8_t *ip_address);

// !arp_find_entry
/*
    Find entry in arp table, if entry does not exists, sends ARP request.
    Timeout: 10 seconds
*/
uint8_t *arp_find_entry(uint8_t *ip_address);

#endif
