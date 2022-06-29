/*
    @JakubPrzystasz
    Created: 12.02.2021
    Modify: 17.02.2021
*/
#ifndef network_ethernet
#define network_ethernet
#include "ethernet_definitions.h"
#include "../../network_utils.h"
#include "../../network_manager.h"

//! network_manager_send_ethernet_frame
/*
    Send ethernet frame
*/
bool network_manager_send_ethernet_frame(ethernet_frame_t *frame, uint32_t data_len);

//! network_manager_make_frame
/*
    Make ethernet frame, with specified data and type
*/
ethernet_frame_t *network_manager_make_frame(uint8_t *src_hw, uint8_t *dst_hw, uint16_t type);

#endif