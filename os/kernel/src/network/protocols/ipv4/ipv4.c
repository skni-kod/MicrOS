#include "ipv4.h"

void ipv4_process_packet(ipv4_packet_t *packet, uint8_t *device_mac)
{
    __ipv4_flip_values(packet);

}

void __ipv4_flip_values(ipv4_packet_t *packet)
{
    *(packet->version_ihl_ptr) = __uint8_flip(*(packet->version_ihl_ptr));

    //TOS

    packet->length = __uint16_flip(packet->length);
    packet->id = __uint16_flip(packet->id);
    // FLAGS AND FRAGMENT OFFSET
    uint16_t tmp = (packet->fragment_offset_high << 8);
    tmp |= packet->fragment_offset_low;
    tmp |= (packet->flags << 13);
    memcpy(packet->flags_fragment_ptr, &tmp, sizeof(uint16_t));

    packet->header_checksum = __uint16_flip(packet->header_checksum);
}