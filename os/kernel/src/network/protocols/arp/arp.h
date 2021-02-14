/*
    @JakubPrzystasz
    Created: 12.02.2021
*/
#ifndef arp_protocol
#define arp_protocol
#include "arp_packet.h"
#include "../../network_utils.h"

//! arp_process_packet
/*
    Process incoming arp packet
*/
void arp_process_packet(arp_packet_t *packet);

#endif
