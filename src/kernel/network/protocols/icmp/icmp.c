#include "icmp.h"

void icmp_process_packet(nic_data_t *data)
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

        // Send response to ECHO REQUEST:

        // TODO: Create ipv4_create_packet, fill with data in icmp datagram, and send with ipv4_send_packet()
        //  in send_packet handle fragmentation
        //  ethernet_frame_t *frame = ethernet_create_frame(
        //      &data->device->interface->mac,
        //      &dst_mac,
        //      IPv4_PROTOCOL_TYPE,
        //      ntohs(packet->length));
        //  {
        //      ipv4_packet_t *reply = (ipv4_packet_t *)frame->data;
        //      memcpy(reply, packet, sizeof(ipv4_packet_t));
        //      memcpy(&reply->dst, &packet->src, IPv4_ADDRESS_LENGTH);
        //      memcpy(&reply->src, &packet->dst, IPv4_ADDRESS_LENGTH);
        //      ipv4_checksum(packet);
        //  }


        // ethernet_send_frame(data->device, ntohs(packet->length), frame);
    }
    break;

    default:
        break;
    }
}
