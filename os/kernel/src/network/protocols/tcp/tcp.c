#include "tcp.h"

void tcp_process_datagram(nic_data_t *data)
{
    ipv4_packet_t *packet = (ipv4_packet_t *)(data->frame + sizeof(ethernet_frame_t));
    tcp_datagram_t *datagram = (tcp_datagram_t *)(data->frame + sizeof(ethernet_frame_t) + sizeof(ipv4_packet_t));
    uint8_t *data_ptr = (uint8_t *)(datagram->options_data + datagram->offset);

    if (datagram->offset > TCP_OPTIONS_OFFSET)
        return 1;

    uint8_t x = tcp_header_size(packet);

    return 1;
}

uint32_t tcp_header_size(ipv4_packet_t *packet)
{
    tcp_datagram_t *datagram = (tcp_datagram_t *)(packet->data);
    return ntohs(packet->length) - (datagram->offset * 8);
}   
