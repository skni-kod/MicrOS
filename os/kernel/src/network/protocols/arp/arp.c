#include "arp.h"

void arp_process_packet(nic_data_t *data)
{
    arp_packet_t *packet = (arp_packet_t *)(data->frame + sizeof(ethernet_frame_t));

    __arp_flip_values(packet);

    switch (packet->opcode)
    {
    case ARP_OPCODE_REQUEST:
        // Handle ARP request
        {
            ethernet_frame_t *frame = ethernet_make_frame(
                data->device->configuration->mac_address,
                packet->src_hw,
                ARP_PROTOCOL_TYPE,
                sizeof(arp_packet_t));

            arp_packet_t *response = (arp_packet_t *)(frame->data);
            response->hardware_type = ARP_HW_TYPE;
            response->opcode = ARP_OPCODE_REPLY;
            response->protocol_type = ARP_PR_TYPE;

            __arp_flip_values(response);

            response->hardware_length = MAC_ADDRESS_LENGTH;
            response->protocol_length = IPv4_ADDRESS_LENGTH;

            memcpy(response->src_hw, data->device->configuration->mac_address, MAC_ADDRESS_SIZE);
            memcpy(response->src_pr, packet->dst_pr, IPv4_ADDRESS_SIZE);

            memcpy(response->dst_hw, packet->src_hw, MAC_ADDRESS_SIZE);
            memcpy(response->dst_pr, packet->src_pr, IPv4_ADDRESS_SIZE);

            ethernet_send_frame(data->device, sizeof(arp_packet_t), frame);

            arp_add_entry(data->device, response->dst_hw, response->dst_pr, ARP_ENTRY_TYPE_DYNAMIC);
        }
        break;
    case ARP_OPCODE_REPLY:
        // Handle ARP reply
        arp_add_entry(data->device, packet->src_hw, packet->src_pr, ARP_ENTRY_TYPE_DYNAMIC);
        break;
    }
}

void arp_add_entry(net_device_t *device, uint8_t *mac_address, uint8_t *ip_address, arp_entry_type_t type)
{
    if (mac_address == 0 || ip_address == 0)
        return;

    // Entry is present
    if (arp_get_entry(device, ip_address))
        return;

    arp_entry_t *entry = heap_kernel_alloc(sizeof(arp_entry_t), 0);

    entry->add_time = get_time();
    entry->type = type;

    memcpy(entry->mac_address, mac_address, MAC_ADDRESS_SIZE);
    memcpy(entry->ip_address, ip_address, IPv4_ADDRESS_SIZE);

    kvector_add(device->configuration->arp_entries, entry);
}

arp_entry_t *arp_get_entry(net_device_t *device, uint8_t *ip_address)
{
    if (!ip_address || !device->configuration->arp_entries->count)
        return 0;

    arp_entry_t *entry;

    for (uint32_t i = 0; i < device->configuration->arp_entries->count; i++)
    {
        entry = ((arp_entry_t *)device->configuration->arp_entries->data[i]);
        if (__ipv4_compare_address(ip_address, entry->ip_address))
            return entry;
    }

    return 0;
}

arp_entry_t *arp_request_entry(net_device_t *device, uint8_t *ip_address)
{
    arp_entry_t *entry = arp_get_entry(device, ip_address);
    if (entry)
        return entry;

    arp_send_request(device, ip_address);

    uint32_t request_time = get_time();

    while (true)
    {
        if ((get_time() - request_time) >= ARP_TIMEOUT)
            return 0;
        if ((get_time() - request_time) % ARP_RETRY_INTERVAL == 0)
        {
            entry = arp_get_entry(device, ip_address);
            if (entry)
                return entry;
            arp_send_request(device, ip_address);
        }
    }
}

void arp_send_request(net_device_t *device, uint8_t *ip_address)
{
    uint8_t broadcast[MAC_ADDRESS_LENGTH] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

    ethernet_frame_t *frame = ethernet_make_frame(
        device->configuration->mac_address,
        broadcast,
        ARP_PROTOCOL_TYPE,
        sizeof(arp_packet_t));

    arp_packet_t *request = (arp_packet_t *)(frame->data);

    request->opcode = ARP_OPCODE_REQUEST;
    request->hardware_type = ARP_HW_TYPE;
    request->protocol_type = ARP_PR_TYPE;
    __arp_flip_values(request);

    request->hardware_length = MAC_ADDRESS_LENGTH;
    request->protocol_length = IPv4_ADDRESS_LENGTH;

    memcpy(request->src_hw, device->configuration->mac_address, MAC_ADDRESS_SIZE);
    memcpy(request->dst_pr, ip_address, IPv4_ADDRESS_SIZE);

    memcpy(request->src_pr, device->configuration->ipv4_address, IPv4_ADDRESS_SIZE);
    __set_mac_addr(request->dst_hw, 0, 0, 0, 0, 0, 0);

    ethernet_send_frame(device, sizeof(arp_packet_t), frame);
}

void __arp_flip_values(arp_packet_t *packet)
{
    packet->opcode = ntohs(packet->opcode);
    packet->hardware_type = ntohs(packet->hardware_type);
    packet->protocol_type = ntohs(packet->protocol_type);
}

bool __arp_compare_mac_address(uint8_t *addr1, uint8_t *addr2)
{
    return !memcmp(addr1, addr2, MAC_ADDRESS_LENGTH);
}