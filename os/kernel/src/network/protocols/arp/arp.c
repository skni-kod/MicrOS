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
        arp_packet_t response;
        response.opcode = __uint16_flip(ARP_OPCODE_REPLY);
        response.hardware_type = __uint16_flip(ARP_HW_TYPE);
        response.protocol_type = __uint16_flip(ARP_PR_TYPE);
        response.hardware_length = MAC_ADDRESS_LENGTH;
        response.protocol_length = IPv4_ADDRESS_LENGTH;

        memcpy(response.src_hw, device_mac, MAC_ADDRESS_SIZE);
        memcpy(response.dst_hw, packet->src_hw, MAC_ADDRESS_SIZE);

        memcpy(response.src_pr, packet->dst_pr, IPv4_ADDRESS_SIZE);
        memcpy(response.dst_pr, packet->src_pr, IPv4_ADDRESS_SIZE);

        ethernet_frame_t *eth_frame = network_manager_make_frame(device_mac, packet->src_hw, ARP_PROTOCOL_TYPE);
        eth_frame->data = (uint8_t *)&response;
        network_manager_send_ethernet_frame(eth_frame, sizeof(arp_packet_t));
        heap_kernel_dealloc(eth_frame);
    }

    //Handle ARP reply
    if (packet->opcode == ARP_OPCODE_REPLY)
    {
    }
}