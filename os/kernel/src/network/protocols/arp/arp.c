#include "arp.h"

void arp_process_packet(arp_packet_t *packet)
{
    packet->hardware_type = __uint16_flip(packet->hardware_type);
    packet->protocol_type = __uint16_flip(packet->protocol_type);
    packet->opcode = __uint16_flip(packet->opcode);
    int x = 2;
}