/*
    @JakubPrzystasz
    Created: 17.02.2021
    Modify:
*/
#ifndef tcp_protocol
#define tcp_protocol

#include "tcp_definitions.h"
#include "../../network_utils.h"
#include "../../network_manager.h"

//! tcp_process_datagram
/*
    Process TCP datagram
*/
void tcp_process_datagram(nic_data_t *data);

//! tcp_checksum
/*
    Returns datagram checksum
*/
uint16_t tcp_checksum(nic_data_t *data);

#endif