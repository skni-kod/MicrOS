/*
    @JakubPrzystasz
    Created: 13.02.2021
*/
#ifndef arp_packet
#define arp_packet
#include <stdint.h>

#define ARP_PROTOCOL_ADDRESS_LENGTH 4
#define ARP_HW_ADDRESS_LENGTH 6
#define ARP_HW_TYPE 0x1
#define ARP_PROTOCOL_TYPE 0x0806

typedef struct arp_packet
{
    uint16_t hardware_type;                      // Hardware type
    uint16_t protocol_type;                      // Protocol type
    uint8_t hardware_length;                     // Hardware address length (Ethernet = 6)
    uint8_t protocol_length;                     // Protocol address length (IPv4 = 4)
    uint16_t opcode;                             // ARP Operation Code
    uint8_t src_hw[ARP_HW_ADDRESS_LENGTH];       // Source hardware address - hlen bytes (see above)
    uint8_t src_pr[ARP_PROTOCOL_ADDRESS_LENGTH]; // Source protocol address - plen bytes (see above). If IPv4 can just be a "u32" type.
    uint8_t dst_hw[ARP_HW_ADDRESS_LENGTH];       // Destination hardware address - hlen bytes (see above)
    uint8_t dst_pr[ARP_PROTOCOL_ADDRESS_LENGTH]; // Destination protocol address - plen bytes (see above). If IPv4 can just be a "u32" type.
} __attribute__((packed)) arp_packet_t;
#endif
