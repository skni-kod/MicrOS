#include "arp.h"

arp_packet_t arp_packet_base = {
    .hardware_type = htons(ARP_HW_TYPE_ETHERNET),
    .protocol_type = htons(ARP_PR_TYPE),
    .hardware_length = sizeof(mac_addr_t),
    .protocol_length = sizeof(ipv4_addr_t),
    .opcode = htons(ARP_OPCODE_REPLY),
};

static arp_entry_t broadcast_entry = {
    .add_time = 0,
    .ip = {
        .oct_a = 0xFF,
        .oct_b = 0xFF,
        .oct_c = 0xFF,
        .oct_d = 0xFF},
    .type = ARP_ENTRY_TYPE_STATIC,
    .mac = {
        .octet_a = 0xFF,
        .octet_b = 0xFF,
        .octet_c = 0xFF,
        .octet_d = 0xFF,
        .octet_e = 0xFF,
        .octet_f = 0xFF,
    }};

uint32_t arp_process_packet(nic_data_t *data)
{
    arp_packet_t *packet = (arp_packet_t *)(data->frame + sizeof(ethernet_frame_t));

    switch (packet->opcode)
    {
    case htons(ARP_OPCODE_REQUEST):
        // Handle ARP request
        if (packet->dst_pr.address == data->device->interface->ipv4_address.address)
        {
            arp_add_entry(data->device, &packet->src_hw, &packet->src_pr, ARP_ENTRY_TYPE_DYNAMIC);

            nic_data_t *reply = ethernet_create_frame(
                data->device,
                ARP_PROTOCOL_TYPE,
                sizeof(arp_packet_t));

            ethernet_frame_t *frame = reply->frame;
            arp_packet_t *response = (arp_packet_t *)(frame->data);
            memcpy(response, &arp_packet_base, sizeof(arp_packet_t));

            memcpy(&response->src_hw, &data->device->interface->mac, sizeof(mac_addr_t));
            memcpy(&response->src_pr, &packet->dst_pr, sizeof(ipv4_addr_t));

            memcpy(&response->dst_hw, &packet->src_hw, sizeof(mac_addr_t));
            memcpy(&response->dst_pr, &packet->src_pr, sizeof(ipv4_addr_t));

            memcpy(&frame->dst, &packet->src_hw, sizeof(mac_addr_t));
            ethernet_send_frame(reply);
        }
        return 1;
    case htons(ARP_OPCODE_REPLY):
        // Handle ARP reply
        arp_add_entry(data->device, &packet->src_hw, &packet->src_pr, ARP_ENTRY_TYPE_DYNAMIC);
        return 1;
    }
    return 0;
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

    // broadcast request
    if ((ipv4_addr_t){.oct_a = 0xFF, .oct_b = 0xFF, .oct_c = 0xFF, .oct_d = 0xFF}.address == ip->address)
        return &broadcast_entry;

    arp_entry_t *entry = arp_get_entry(device, ip);
    if (entry)
        return entry;

    arp_send_request(device, ip);

    return 0;

    // uint32_t request_time = get_time();

    // while (true)
    // {
    //     if ((get_time() - request_time) >= ARP_TIMEOUT)
    //         return 0;
    //     if ((get_time() - request_time) % ARP_RETRY_INTERVAL == 0)
    //     {
    //         entry = arp_get_entry(device, ip);
    //         if (entry)
    //             return entry;
    //         arp_send_request(device, ip);
    //     }
    // }
}

void arp_send_request(net_device_t *device, ipv4_addr_t *ip)
{
    mac_addr_t broadcast = {.octet_a = 0xFF,
                            .octet_b = 0xFF,
                            .octet_c = 0xFF,
                            .octet_d = 0xFF,
                            .octet_e = 0xFF,
                            .octet_f = 0xFF};

    nic_data_t *request = ethernet_create_frame(
        device,
        ARP_PROTOCOL_TYPE,
        sizeof(arp_packet_t));

    ethernet_frame_t *frame = request->frame;
    {
        arp_packet_t *request = (arp_packet_t *)(frame->data);

        request->opcode = htons(ARP_OPCODE_REQUEST);
        request->hardware_type = htons(ARP_HW_TYPE_ETHERNET);
        request->protocol_type = htons(ARP_PR_TYPE);

        request->hardware_length = sizeof(mac_addr_t);
        request->protocol_length = sizeof(ipv4_addr_t);

        memcpy(&request->src_hw, &device->interface->mac, sizeof(mac_addr_t));
        memcpy(&request->dst_pr, ip, sizeof(ipv4_addr_t));

        memcpy(&request->src_pr, &device->interface->ipv4_address, sizeof(ipv4_addr_t));

        request->dst_hw = (mac_addr_t){.octet_a = 0,
                                       .octet_b = 0,
                                       .octet_c = 0,
                                       .octet_d = 0,
                                       .octet_e = 0,
                                       .octet_f = 0};
    }

    memcpy(&frame->dst, &broadcast, sizeof(mac_addr_t));
    ethernet_send_frame(request);
}
