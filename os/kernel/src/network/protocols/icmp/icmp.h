/*
    @JakubPrzystasz
    Created: 30.06.2022
*/
#ifndef icmp_protocol
#define icmp_protocol

#include "icmp_definitions.h"
#include "../../network_utils.h"
#include "../../network_manager.h"

void icmp_process_packet(nic_data_t *data);


#endif