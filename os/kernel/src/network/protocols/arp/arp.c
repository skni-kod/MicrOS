#include "arp.h"

void arp_process_packet(arp_packet_t *packet, uint8_t *device_mac)
{
    //Prevent from processing broadcast packet multiple times
    if (!__compare_mac_address(network_manager_verify_ipv4_address(packet->dst_pr), device_mac))
        return;

    packet->hardware_type = __uint16_flip(packet->hardware_type);
    packet->protocol_type = __uint16_flip(packet->protocol_type);
    packet->opcode = __uint16_flip(packet->opcode);

    //Handle ARP request
    if (packet->opcode == ARP_OPCODE_REQUEST)
    {
        uint32_t response_len = sizeof(ethernet_frame_t) + sizeof(arp_packet_t) - sizeof(void *);

        arp_packet_t response;
        response.opcode = __uint16_flip(ARP_OPCODE_REPLY);
        response.hardware_type = __uint16_flip(0x1);
        response.protocol_type = __uint16_flip(0x0800);
        response.hardware_length = __uint16_flip(MAC_ADDRESS_LENGTH);
        response.protocol_type = __uint16_flip(IPv4_ADDRESS_LENGTH);

        ethernet_frame_t *eth_frame = network_manager_make_frame(device_mac, packet->src_hw, ARP_PROTOCOL_TYPE);
        eth_frame->data = (uint8_t *)&response;
        network_manager_send_ethernet_frame(eth_frame, response_len);
        heap_kernel_dealloc(eth_frame);
    }

    //Handle ARP reply
    if (packet->opcode == ARP_OPCODE_REPLY)
    {
    }
}