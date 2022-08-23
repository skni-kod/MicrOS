/*
    @JakubPrzystasz
    Created: 17.02.2021
    Modify:
*/
#ifndef udp_protocol
#define udp_protocol

#include "udp_definitions.h"
#include "../../network_utils.h"
#include "../../network_manager.h"

//! udp_process_datagram
/*
    Process UDP datagram
*/
void udp_process_datagram(nic_data_t *data);

//! udp_checksum
/*
    Returns datagram checksum
*/
uint16_t udp_checksum(nic_data_t *packet);

uint16_t compute_udp_checksum(nic_data_t *data);

//! __udp_flip_values
/*
    Flip values
*/
void __udp_flip_values(udp_datagram_t *datagram);

#endif