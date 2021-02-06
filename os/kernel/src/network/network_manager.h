/*
    @JakubPrzystasz
    Created: 06.02.2021
*/
#ifndef network_manager
#define network_manager

#include "../klibrary/kvector.h"
#include "../drivers/nic/rtl8139/rtl8139.h"
#include "net_device.h"

//! network_manager_init
/*
    Initialize network manager
*/
bool network_manager_init();

//! network_manager_send_packet
/*
    Send packet
*/
void netwok_manager_send_packet(net_packet_t *packet);

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

//! __network_manager_print_device_info
/*
    Print device info
*/
void __network_manager_print_device_info(net_device_t *device);

//! __network_manager_compare_mac_address
/*
    Returns true if given MAC adresses are equal
*/
bool __network_manager_compare_mac_address(uint8_t *first, uint8_t *second);

#endif