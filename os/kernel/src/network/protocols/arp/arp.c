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
            heap_kernel_verify_integrity();
            ethernet_frame_t *frame = ethernet_make_frame(
                data->device->configuration->mac_address,
                packet->src_hw,
                ARP_PROTOCOL_TYPE,
                sizeof(arp_packet_t));

            arp_packet_t *response = (arp_packet_t *)((void*)frame->data);
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

            ethernet_send_frame(data->device,sizeof(arp_packet_t),frame);

            arp_add_entry(data->device, response->dst_hw, response->dst_pr);
        }
        break;
    case ARP_OPCODE_REPLY:
        // Handle ARP reply
        arp_add_entry(data->device, packet->src_hw, packet->src_pr);
        break;
    }
}

void arp_add_entry(net_device_t *device, uint8_t *mac_address, uint8_t *ip_address)
{
    char str[128] = "";
    kernel_sprintf(str,"New ARP entry: %d %d %d %d",ip_address[0],ip_address[1],ip_address[2],ip_address[3]);
    logger_log_info(str);
    // if (mac_address == 0 || ip_address == 0)
    //     return;

    // arp_entry_t *new_entry = heap_kernel_alloc(sizeof(arp_entry_t), 0);

    // new_entry->income_time = get_time();
    // new_entry->type = ARP_ENTRY_DYNAMIC;

    // memcpy(new_entry->mac_address, mac_address, MAC_ADDRESS_SIZE);
    // memcpy(new_entry->ip_address, ip_address, IPv4_ADDRESS_SIZE);

    // kvector_add(arp_table, new_entry);
}

// uint8_t *arp_find_entry(uint8_t *ip_address)
// {
//     if (ip_address == 0 || arp_table == 0)
//         return 0;

//     if (arp_table->count < 1)
//         return 0;

//     arp_entry_t *ptr;
//     for (uint8_t i = 0; i < arp_table->count; i++)
//     {
//         ptr = ((arp_entry_t *)arp_table->data[i]);
//         if (__compare_ip_address(ip_address, ptr->ip_address))
//         {
//             uint8_t *ret = heap_kernel_alloc(MAC_ADDRESS_SIZE, 0);
//             memcpy(ret, ptr->mac_address, MAC_ADDRESS_SIZE);
//             return ret;
//         }
//     }

//     return 0;
// }

// uint8_t *arp_get_entry(uint8_t *ip_address)
// {
//     if (ip_address == 0)
//         return 0;

//     uint8_t *ret = arp_find_entry(ip_address);

//     if (ret != 0)
//         return ret;

//     uint32_t request_time = get_time();

//     arp_send_request(ip_address);

//     while (true)
//     {
//         if ((get_time() - request_time) >= ARP_TIMEOUT)
//             break;
//         if ((get_time() - request_time) % ARP_RETRY_INTERVAL == 0)
//         {
//             ret = arp_find_entry(ip_address);
//             if (ret != 0)
//                 return ret;
//             arp_send_request(ip_address);
//         }
//     }

//     return 0;
// }

// void arp_send_request(uint8_t *ip_address)
// {
//     net_device_t *nic = network_manager_get_nic();
//     if (nic == 0)
//         return;

//     arp_packet_t request;
//     request.opcode = ARP_OPCODE_REQUEST;
//     request.hardware_type = ARP_HW_TYPE;
//     request.protocol_type = ARP_PR_TYPE;
//     __arp_flip_values(&request);

//     request.hardware_length = MAC_ADDRESS_LENGTH;
//     request.protocol_length = IPv4_ADDRESS_LENGTH;

//     memcpy(request.src_hw, nic->mac_address, MAC_ADDRESS_SIZE);

//     __set_mac_addr(request.dst_hw, 0, 0, 0, 0, 0, 0);

//     memcpy(request.src_pr, nic->ipv4_address, IPv4_ADDRESS_SIZE);
//     memcpy(request.dst_pr, ip_address, IPv4_ADDRESS_SIZE);

//     uint8_t dst_mac_addr[MAC_ADDRESS_LENGTH];
//     __set_mac_addr(dst_mac_addr, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff);

//     ethernet_frame_t *eth_frame = network_manager_make_frame(nic->mac_address, dst_mac_addr, ARP_PROTOCOL_TYPE);
//     eth_frame->data = (uint8_t *)&request;
//     network_manager_send_ethernet_frame(eth_frame, sizeof(arp_packet_t));
//     heap_kernel_dealloc(eth_frame);
// }

void __arp_flip_values(arp_packet_t *packet)
{
    packet->opcode = __uint16_flip(packet->opcode);
    packet->hardware_type = __uint16_flip(packet->hardware_type);
    packet->protocol_type = __uint16_flip(packet->protocol_type);
}

bool __arp_compare_mac_address(uint8_t *addr1, uint8_t *addr2)
{
    return !memcmp(addr1, addr2, MAC_ADDRESS_LENGTH);
}