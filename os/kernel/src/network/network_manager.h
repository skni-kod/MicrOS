/*
    @JakubPrzystasz
    Created: 06.02.2021
*/
#ifndef network_manager
#define network_manager

#include "../drivers/nic/rtl8139/rtl8139.h"
#include "../drivers/nic/virtio-pci-nic/virtio-pci-nic.h"

#include "../logger/logger.h"
#include "network_definitions.h"
#include "protocols/protocols.h"
#include "network_utils.h"

//! network_manager_init
/*
    Initialize network manager
*/
bool network_manager_init();

//! network_manager_send_packet
/*
    Send packet, it can be just "HelloWorld!"
*/
void network_manager_send_packet(net_packet_t *packet);

//! network_manager_receive_packet
/*
    Function triggers when packet incomes to OS,
    given packet must be on heap
*/
void network_manager_receive_packet(net_packet_t *packet);

//! network_manager_process_packet
/*
    Process incoming packet
*/
void network_manager_process_packet(net_packet_t *packet);

//! network_manager_bytes_sent
/*
    Returns number of bytes sent over Ethernet
*/
uint64_t network_manager_bytes_sent(void);

//! network_manager_bytes_received
/*
    Returns number of bytes received over Ethernet
*/
uint64_t network_manager_bytes_received(void);

//! network_manager_verify_ipv4_address
/*
    Returns MAC address of NIC that has IPv4 address passed in arg
*/
uint8_t *network_manager_verify_ipv4_address(uint8_t *ipv4_address);

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

//! network_manager_get_machine_ip
/*
    Returns main NIC
*/
net_device_t *network_manager_get_nic();

//! __network_manager_print_device_info
/*
    Print device info
*/
void __network_manager_print_device_info(net_device_t *device);

#endif