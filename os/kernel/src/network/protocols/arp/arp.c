#include "arp.h"

kvector *arp_table = 0;

void arp_process_packet(arp_packet_t *packet, uint8_t *device_mac)
{
    // Prevent from processing broadcast packet multiple times
    if (!__compare_mac_address(network_manager_verify_ipv4_address(packet->dst_pr), device_mac))
        return;

    __arp_flip_values(packet);

    switch (packet->opcode)
    {
        // Handle ARP request
        case ARP_OPCODE_REQUEST:
        {
            arp_packet_t response;
            response.opcode = ARP_OPCODE_REPLY;
            response.hardware_type = ARP_HW_TYPE;
            response.protocol_type = ARP_PR_TYPE;
            __arp_flip_values(&response);

            response.hardware_length = MAC_ADDRESS_LENGTH;
            response.protocol_length = IPv4_ADDRESS_LENGTH;

            memcpy(response.src_hw, device_mac, MAC_ADDRESS_SIZE);
            memcpy(response.dst_hw, packet->src_hw, MAC_ADDRESS_SIZE);

            memcpy(response.src_pr, packet->dst_pr, IPv4_ADDRESS_SIZE);
            memcpy(response.dst_pr, packet->src_pr, IPv4_ADDRESS_SIZE);

            ethernet_frame_t *eth_frame = network_manager_make_frame(device_mac, packet->src_hw, ARP_PROTOCOL_TYPE);
            eth_frame->data = (uint8_t *)&response;
            network_manager_send_ethernet_frame(eth_frame, sizeof(arp_packet_t));

            arp_add_entry(response.dst_hw, response.dst_pr);

            heap_kernel_dealloc(eth_frame);
        }
        break;
        // Handle ARP reply
        case ARP_OPCODE_REPLY:
        {
            arp_add_entry(packet->src_hw, packet->src_pr);
        }
        break;
    }
}

void arp_add_entry(uint8_t *mac_address, uint8_t *ip_address)
{
    if (mac_address == 0 || ip_address == 0)
        return;

    // Initialize table
    if (arp_table == 0)
    {
        arp_table = heap_kernel_alloc(sizeof(kvector), 0);
        kvector_init(arp_table);
    }

    arp_entry_t *new_entry = heap_kernel_alloc(sizeof(arp_entry_t), 0);

    new_entry->income_time = get_time();
    new_entry->type = ARP_ENTRY_DYNAMIC;

    memcpy(new_entry->mac_address, mac_address, MAC_ADDRESS_SIZE);
    memcpy(new_entry->ip_address, ip_address, IPv4_ADDRESS_SIZE);

    kvector_add(arp_table, new_entry);
}

uint8_t *arp_find_entry(uint8_t *ip_address)
{
    if (ip_address == 0 || arp_table == 0)
        return 0;

    if (arp_table->count < 1)
        return 0;

    arp_entry_t *ptr;
    for (uint8_t i = 0; i < arp_table->count; i++)
    {
        ptr = ((arp_entry_t *)arp_table->data[i]);
        if (__compare_ip_address(ip_address, ptr->ip_address))
        {
            uint8_t *ret = heap_kernel_alloc(MAC_ADDRESS_SIZE, 0);
            memcpy(ret, ptr->mac_address, MAC_ADDRESS_SIZE);
            return ret;
        }
    }

    return 0;
}

uint8_t *arp_get_entry(uint8_t *ip_address)
{
    if (ip_address == 0)
        return 0;

    uint8_t *ret = arp_find_entry(ip_address);

    if (ret != 0)
        return ret;

    uint32_t request_time = get_time();

    arp_send_request(ip_address);

    while (true)
    {
        if ((get_time() - request_time) >= ARP_TIMEOUT)
            break;
        if ((get_time() - request_time) % ARP_RETRY_INTERVAL == 0)
        {
            ret = arp_find_entry(ip_address);
            if (ret != 0)
                return ret;
            arp_send_request(ip_address);
        }
    }

    return 0;
}

void arp_send_request(uint8_t *ip_address)
{
    net_device_t *nic = network_manager_get_nic();
    if (nic == 0)
        return;

    arp_packet_t request;
    request.opcode = ARP_OPCODE_REQUEST;
    request.hardware_type = ARP_HW_TYPE;
    request.protocol_type = ARP_PR_TYPE;
    __arp_flip_values(&request);

    request.hardware_length = MAC_ADDRESS_LENGTH;
    request.protocol_length = IPv4_ADDRESS_LENGTH;

    memcpy(request.src_hw, nic->mac_address, MAC_ADDRESS_SIZE);

    __set_mac_addr(request.dst_hw, 0, 0, 0, 0, 0, 0);

    memcpy(request.src_pr, nic->ipv4_address, IPv4_ADDRESS_SIZE);
    memcpy(request.dst_pr, ip_address, IPv4_ADDRESS_SIZE);

    uint8_t dst_mac_addr[MAC_ADDRESS_LENGTH];
    __set_mac_addr(dst_mac_addr, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff);

    ethernet_frame_t *eth_frame = network_manager_make_frame(nic->mac_address, dst_mac_addr, ARP_PROTOCOL_TYPE);
    eth_frame->data = (uint8_t *)&request;
    network_manager_send_ethernet_frame(eth_frame, sizeof(arp_packet_t));
    heap_kernel_dealloc(eth_frame);
}

void __arp_flip_values(arp_packet_t *packet)
{
    if (packet != 0)
    {
        packet->opcode = __uint16_flip(packet->opcode);
        packet->hardware_type = __uint16_flip(packet->hardware_type);
        packet->protocol_type = __uint16_flip(packet->protocol_type);
    }
}