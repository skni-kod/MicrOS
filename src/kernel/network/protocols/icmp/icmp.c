#include "icmp.h"

void icmp_process_packet(nic_data_t *data)
{
    ipv4_packet_t *packet = (ipv4_packet_t *)(data->frame + sizeof(ethernet_frame_t));
    icmp_header_t *header = (icmp_header_t *)(data->frame + sizeof(ethernet_frame_t) + sizeof(ipv4_packet_t));

    switch (header->type)
    {
    case ICMP_TYPE_ECHO_REQUEST:
    {
        // Send response to ECHO REQUEST:

        //TODO: Create ipv4_create_packet, fill with data in icmp datagram, and send with ipv4_send_packet()
        // in send_packet handle fragmentation

        // nic_data_t *request = ethernet_create_frame(
        //     device,
        //     ARP_PROTOCOL_TYPE,
        //     sizeof(arp_packet_t));

        // ethernet_frame_t *frame = request->frame;
        // {

        //     request->dst_hw.octet_f = 0;
        // }

        // memcpy(&frame->dst, &broadcast, sizeof(mac_addr_t));
        // ethernet_send_frame(request, 0);
        // arp_entry_t *entry = arp_get_entry(data->device, &packet->src);

        // if (!entry)
        //     return 0;

        // mac_addr_t dst_mac = entry->mac;

        // echo request
        // ethernet_frame_t *frame = ethernet_create_frame(
        //     &data->device->interface->mac,
        //     &dst_mac,
        //     IPv4_PROTOCOL_TYPE,
        //     ntohs(packet->length));
        // {
        //     ipv4_packet_t *reply = (ipv4_packet_t *)frame->data;
        //     memcpy(reply, packet, sizeof(ipv4_packet_t));
        //     memcpy(&reply->dst, &packet->src, IPv4_ADDRESS_LENGTH);
        //     memcpy(&reply->src, &packet->dst, IPv4_ADDRESS_LENGTH);
        //     ipv4_checksum(packet);
        // }

        // {
        //     icmp_header_t *reply = (icmp_header_t *)(frame->data + sizeof(ipv4_packet_t));
        //     reply->type = ICMP_TYPE_ECHO_REPLY;
        //     reply->code = ICMP_CODE_NO_CODE;
        //     memcpy(reply->data, header->data, ntohs(packet->length) - (sizeof(ipv4_packet_t) + sizeof(icmp_header_t)));
        //     reply->checksum = 0;
        //     reply->checksum = __ip_wrapsum(__ip_checksum((uint8_t *)reply, ntohs(packet->length) - sizeof(ipv4_packet_t), 0U));
        // }
        // ethernet_send_frame(data->device, ntohs(packet->length), frame);
    }
    break;

    default:
        break;
    }
}
