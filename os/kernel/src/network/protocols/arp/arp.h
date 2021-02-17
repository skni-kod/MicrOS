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

#endif
