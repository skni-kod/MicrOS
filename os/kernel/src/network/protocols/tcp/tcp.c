#include "tcp.h"

void tcp_process_datagram(nic_data_t *data)
{
    // ipv4_packet_t *packet = (ipv4_packet_t *)(data->frame + sizeof(ethernet_frame_t));
    // tcp_datagram_t *datagram = (tcp_datagram_t *)(data->frame + sizeof(ethernet_frame_t) + sizeof(ipv4_packet_t));
    // uint8_t *data_ptr = (uint8_t *)(datagram->options_data + datagram->offset);

    // if (datagram->offset > TCP_OPTIONS_OFFSET)
    //     return 1;

    // // echo request
    // ethernet_frame_t *frame = ethernet_make_frame(
    //     data->device->configuration->mac_address,
    //     ((ethernet_frame_t *)data->frame)->src_mac_addr,
    //     IPv4_PROTOCOL_TYPE,
    //     htons(packet->length));
    // {
    //     ipv4_packet_t *reply = (ipv4_packet_t *)frame->data;
    //     memcpy(reply, packet, sizeof(ipv4_packet_t));
    //     memcpy(reply->dst_ip, packet->src_ip, IPv4_ADDRESS_LENGTH);
    //     memcpy(reply->src_ip, packet->dst_ip, IPv4_ADDRESS_LENGTH);
    //     reply->protocol = IP_PROTOCOL_TCP;
    // }

    // {
    //     tcp_datagram_t *reply = (tcp_datagram_t *)(frame->data + sizeof(ipv4_packet_t));
    //     memcpy(reply, packet, sizeof(tcp_datagram_t));
    //     reply->dst_port = datagram->src_port;
    //     reply->src_port = datagram->dst_port;
    //     memcpy(reply->options_data, datagram->options_data, tcp_data_size(packet));
    // }
    // nic_data_t tmp;
    // tmp.frame = frame;
    // __ip_tcp_udp_checksum(&tmp);
    // datagram->checksum = __uint16_flip(datagram->checksum);
    // ethernet_send_frame(data->device, htons(packet->length), frame);

    // return 1;
}

uint32_t tcp_data_size(ipv4_packet_t *packet)
{
    tcp_datagram_t *datagram = (tcp_datagram_t *)(packet->data);
    return ntohs(packet->length) - (datagram->offset * 8);
}
