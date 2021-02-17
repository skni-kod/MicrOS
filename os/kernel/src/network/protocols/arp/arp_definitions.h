/*
    @JakubPrzystasz
    Created: 13.02.2021
    Modify: 17.02.2021
*/
#ifndef network_arp_definitions
#define network_arp_definitions

#include "../../network_definitions.h"
#include <stdint.h>

#define ARP_PROTOCOL_TYPE 0x0806
#define ARP_OPCODE_REQUEST 0x1
#define ARP_OPCODE_REPLY 0x2
#define ARP_HW_TYPE 0x1
#define ARP_PR_TYPE 0x0800

#define ARP_ENTRY_DYNAMIC 0x1
#define ARP_ENTRY_STATIC 0x2

#define ARP_TIMEOUT 5000
#define ARP_RETRY_INTERVAL 200

typedef struct arp_packet
{
    uint16_t hardware_type;              // Hardware type
    uint16_t protocol_type;              // Protocol type
    uint8_t hardware_length;             // Hardware address length (Ethernet = 6)
    uint8_t protocol_length;             // Protocol address length (IPv4 = 4)
    uint16_t opcode;                     // ARP Operation Code
    uint8_t src_hw[MAC_ADDRESS_LENGTH];  // Source hardware address - hlen bytes (see above)
    uint8_t src_pr[IPv4_ADDRESS_LENGTH]; // Source protocol address - plen bytes (see above). If IPv4 can just be a "u32" type.
    uint8_t dst_hw[MAC_ADDRESS_LENGTH];  // Destination hardware address - hlen bytes (see above)
    uint8_t dst_pr[IPv4_ADDRESS_LENGTH]; // Destination protocol address - plen bytes (see above). If IPv4 can just be a "u32" type.
} __attribute__((packed)) arp_packet_t;

typedef struct arp_entry
{
    uint8_t mac_address[MAC_ADDRESS_LENGTH];
    uint8_t ip_address[IPv4_ADDRESS_LENGTH];
    uint32_t income_time;
    uint8_t type;
} arp_entry_t;

#endif
