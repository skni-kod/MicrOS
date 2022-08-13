#include "icmp.h"

void icmp_process_packet(nic_data_t *data)
{
    ipv4_packet_t *packet = (ipv4_packet_t *)(data->frame + sizeof(ethernet_frame_t));
    icmp_header_t *header = (icmp_header_t *)(data->frame + sizeof(ethernet_frame_t) + sizeof(ipv4_packet_t));

    if (header->type == 8)
    {
        uint8_t *dst_mac = arp_request_entry(data->device, packet->src_ip)->mac_address;
        if (dst_mac)
        {
            // echo request
            ethernet_frame_t *frame = ethernet_make_frame(
                data->device->configuration->mac_address,
                dst_mac,
                IPv4_PROTOCOL_TYPE,
                packet->length);
            {
                ipv4_packet_t *reply = (ipv4_packet_t *)frame->data;
                memcpy(reply, packet, sizeof(ipv4_packet_t));
                __ipv4_flip_values(reply);
                memcpy(reply->dst_ip, packet->src_ip, IPv4_ADDRESS_LENGTH);
                memcpy(reply->src_ip, packet->dst_ip, IPv4_ADDRESS_LENGTH);
            }

            {
                icmp_header_t *reply = (icmp_header_t *)(frame->data + sizeof(ipv4_packet_t));
                reply->type = 0;
                reply->code = 0;
                reply->checksum = 0;
                reply->checksum = ~(reply->type | reply->code | reply->checksum);
                memcpy(reply->data, header->data, packet->length - (sizeof(ipv4_packet_t) + sizeof(icmp_header_t)));
            }
            ethernet_send_frame(data->device, packet->length, frame);
        }
    }
}