#include "arp.h"

arp_packet_t arp_packet_base = {
    .hardware_type = ARP_HW_TYPE_ETHERNET,
    .protocol_type = ARP_PR_TYPE,
    .hardware_length = MAC_ADDRESS_LENGTH,
    .protocol_length = IPv4_ADDRESS_LENGTH,
    .opcode = ARP_OPCODE_REPLY,
};

void arp_process_packet(nic_data_t *data)
{
    arp_packet_t *packet = (arp_packet_t *)(data->frame + sizeof(ethernet_frame_t));

    __arp_flip_values(packet);

    switch (packet->opcode)
    {
    case ARP_OPCODE_REQUEST:
        // Handle ARP request
        if (packet->dst_pr.address == data->device->interface->ipv4.address)
        {
            ethernet_frame_t *frame = ethernet_create_frame(
                &data->device->interface->mac,
                &packet->src_hw,
                ARP_PROTOCOL_TYPE,
                sizeof(arp_packet_t));

            arp_packet_t *response = (arp_packet_t *)(frame->data);
            memcpy(response, &arp_packet_base, sizeof(arp_packet_t));
            __arp_flip_values(response);

            memcpy(&response->src_hw, &data->device->interface->mac, sizeof(mac_addr_t));
            memcpy(&response->src_pr, &packet->dst_pr, sizeof(ipv4_addr_t));

            memcpy(&response->dst_hw, &packet->src_hw, sizeof(mac_addr_t));
            memcpy(&response->dst_pr, &packet->src_pr, sizeof(ipv4_addr_t));

            ethernet_send_frame(data->device, sizeof(arp_packet_t), frame);

            arp_add_entry(data->device, &response->dst_hw, &response->dst_pr, ARP_ENTRY_TYPE_DYNAMIC);
        }
        break;
    case ARP_OPCODE_REPLY:
        // Handle ARP reply
        arp_add_entry(data->device, &packet->src_hw, &packet->src_pr, ARP_ENTRY_TYPE_DYNAMIC);
        break;
    }
}

void arp_add_entry(net_device_t *device, mac_addr_t *mac, ipv4_addr_t *ip, arp_entry_type_t type)
{
    if (mac == 0 || ip == 0)
        return;

    // Entry is present
    if (arp_get_entry(device, ip))
        return;

    arp_entry_t *entry = heap_kernel_alloc(sizeof(arp_entry_t), 0);

    entry->add_time = get_time();
    entry->type = type;

    memcpy(&entry->mac, mac, sizeof(mac_addr_t));
    memcpy(&entry->ip, ip, sizeof(ipv4_addr_t));

    kvector_add(device->interface->arp_entries, entry);
}

arp_entry_t *arp_get_entry(net_device_t *device, ipv4_addr_t *ip)
{
    if (!ip || !device->interface->arp_entries->count)
        return 0;

    arp_entry_t *entry;

    for (uint32_t i = 0; i < device->interface->arp_entries->count; i++)
    {
        entry = ((arp_entry_t *)device->interface->arp_entries->data[i]);
        if (ip->address == entry->ip.address)
            return entry;
    }

    return 0;
}

arp_entry_t *arp_request_entry(net_device_t *device, ipv4_addr_t *ip)
{
    if (!ip)
        return 0;

    arp_entry_t *entry = arp_get_entry(device, ip);
    if (entry)
        return entry;

    arp_send_request(device, ip);

    uint32_t request_time = get_time();

    while (true)
    {
        if ((get_time() - request_time) >= ARP_TIMEOUT)
            return 0;
        if ((get_time() - request_time) % ARP_RETRY_INTERVAL == 0)
        {
            entry = arp_get_entry(device, ip);
            if (entry)
                return entry;
            arp_send_request(device, ip);
        }
    }
}

void arp_send_request(net_device_t *device, ipv4_addr_t *ip)
{
    uint8_t broadcast[MAC_ADDRESS_LENGTH] = {[0 ...(MAC_ADDRESS_LENGTH - 1)] = 0xFF};

    ethernet_frame_t *frame = ethernet_create_frame(
        &device->interface->mac,
        broadcast,
        ARP_PROTOCOL_TYPE,
        sizeof(arp_packet_t));

    arp_packet_t *request = (arp_packet_t *)(frame->data);

    request->opcode = ARP_OPCODE_REQUEST;
    request->hardware_type = ARP_HW_TYPE_ETHERNET;
    request->protocol_type = ARP_PR_TYPE;
    __arp_flip_values(request);

    request->hardware_length = MAC_ADDRESS_LENGTH;
    request->protocol_length = IPv4_ADDRESS_LENGTH;

    memcpy(&request->src_hw, &device->interface->mac, sizeof(mac_addr_t));
    memcpy(&request->dst_pr, ip, sizeof(ipv4_addr_t));

    memcpy(&request->src_pr, &device->interface->ipv4, sizeof(ipv4_addr_t));

    request->dst_hw.octet_a = 0,
    request->dst_hw.octet_b = 0,
    request->dst_hw.octet_c = 0,
    request->dst_hw.octet_d = 0,
    request->dst_hw.octet_e = 0,
    request->dst_hw.octet_f = 0;

    ethernet_send_frame(device, sizeof(arp_packet_t), frame);
}

void __arp_flip_values(arp_packet_t *packet)
{
    packet->opcode = ntohs(packet->opcode);
    packet->hardware_type = ntohs(packet->hardware_type);
    packet->protocol_type = ntohs(packet->protocol_type);
}
