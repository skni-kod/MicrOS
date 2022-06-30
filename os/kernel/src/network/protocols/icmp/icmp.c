#include "icmp.h"

void icmp_process_packet(nic_data_t *data)
{
    ipv4_packet_t *packet = (ipv4_packet_t *)(data->frame + sizeof(ethernet_frame_t));
    icmp_header_t *header = (icmp_header_t *)(data->frame + sizeof(ethernet_frame_t) + sizeof(ipv4_packet_t));

    if (header->type == 8)
    {
        // echo request

        ethernet_frame_t *frame = ethernet_make_frame(
            data->device->configuration->mac_address,
            arp_get_entry(data->device, packet->dst_ip)->mac_address,
            IPv4_PROTOCOL_TYPE,
            sizeof(ipv4_packet_t) + sizeof(icmp_header_t) + 4);
        {
            icmp_header_t *reply = (icmp_header_t *)(frame->data + sizeof(sizeof(ipv4_packet_t)));
            reply->type = 0;
            reply->code = 0;
            reply->checksum = 0;
            reply->checksum = ~(reply->type | reply->code | reply->checksum);
        }
        {
            ipv4_packet_t *reply = (ipv4_packet_t *)frame->data;
            memcpy(reply, packet, sizeof(ipv4_packet_t));
            memcpy(reply->dst_ip, packet->src_ip, IPv4_ADDRESS_LENGTH);
            memcpy(reply->src_ip, packet->dst_ip, IPv4_ADDRESS_LENGTH);
        }

        ethernet_send_frame(data->device, sizeof(ipv4_packet_t) + sizeof(icmp_header_t), frame);
    }
}