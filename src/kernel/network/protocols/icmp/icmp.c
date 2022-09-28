#include "icmp.h"

uint32_t icmp_process_packet(nic_data_t *data)
{
    ipv4_packet_t *packet = (ipv4_packet_t *)(data->frame + sizeof(ethernet_frame_t));
    icmp_header_t *header = (icmp_header_t *)(data->frame + sizeof(ethernet_frame_t) + sizeof(ipv4_packet_t));

    switch (header->type)
    {
    case ICMP_TYPE_ECHO_REQUEST:
    {
        icmp_header_echo_t *request = header->data;
        uint32_t data_size = ntohs(packet->length) - sizeof(ipv4_packet_t);
        nic_data_t *response = ipv4_create_packet(data->device, IP_PROTOCOL_ICMP, packet->src, data_size);

        {
            icmp_header_t *reply = (icmp_header_t *)(response->frame + sizeof(ethernet_frame_t) + sizeof(ipv4_packet_t));
            reply->type = ICMP_TYPE_ECHO_REPLY;
            reply->code = ICMP_CODE_NO_CODE;
            memcpy(reply->data, header->data, ntohs(packet->length) - (sizeof(ipv4_packet_t) + sizeof(icmp_header_t)));
            reply->checksum = 0;
            reply->checksum = __ip_wrapsum(__ip_checksum((uint8_t *)reply, ntohs(packet->length) - sizeof(ipv4_packet_t), 0U));
        }
        ipv4_send_packet(response);
        return 1;
    }
    break;

    default:
        return 0;
    }
}
