/*
    @JakubPrzystasz
    Created: 12.02.2021
    Modify: 17.02.2021
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

//! arp_add_entry
/*
    Add arp entry to vector
*/
void arp_add_entry(uint8_t *mac_address, uint8_t *ip_address);

//! arp_find_entry
/*
    Find entry in arp table
*/
uint8_t *arp_find_entry(uint8_t *ip_address);

//! arp_get_entry
/*
    Returns MAC address of desired IP.
    If not present in arp table, sends request, and waits up to ARP_TIMEOUT
*/
uint8_t *arp_get_entry(uint8_t *ip_address);

//! arp_send_request
/*
    Send ARP request
*/
void arp_send_request(uint8_t *ip_address);

//! __arp_flip_values
/*
    Flip literal value
*/
void __arp_flip_values(arp_packet_t *packet);

#endif
