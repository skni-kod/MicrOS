#include "ipv4.h"

void ipv4_process_packet(nic_data_t *data)
{
    ipv4_packet_t *packet = (ipv4_packet_t *)(data->frame + sizeof(ethernet_frame_t));
    __ipv4_flip_values(packet);

    // TOS
    // https://datatracker.ietf.org/doc/html/rfc2474
    // https://www.iana.org/assignments/dscp-registry/dscp-registry.xhtml

    switch (packet->protocol)
    {
    case IP_PROTOCOL_ICMP:
    {
        icmp_process_packet(data);
    }
    break;
    case IP_PROTOCOL_UDP:
        break;
    default:
        break;
    }
}

void __ipv4_flip_values(ipv4_packet_t *packet)
{
    *(packet->version_ihl_ptr) = __uint8_flip(*(packet->version_ihl_ptr));

    packet->length = __uint16_flip(packet->length);
    packet->id = __uint16_flip(packet->id);

    // FLAGS AND FRAGMENT OFFSET
    uint16_t tmp = (packet->fragment_offset_high << 8);
    tmp |= packet->fragment_offset_low;
    tmp |= (packet->flags << 13);
    memcpy(packet->flags_fragment_ptr, &tmp, sizeof(uint16_t));

    packet->header_checksum = __uint16_flip(packet->header_checksum);
}

bool __ipv4_compare_address(uint8_t *addr1, uint8_t *addr2)
{
    return !memcmp(addr1, addr2, IPv4_ADDRESS_LENGTH);
}