/*
    @JakubPrzystasz
    Created: 06.02.2021
*/

#include "network_manager.h"

kvector *net_devices;
uint64_t bytes_sent_count = 0;
uint64_t bytes_received_count = 0;

//TODO:
/*
    When some day, multithreading will be on the agenda
    there is some tasks to do:
    - enable buffering TX and RX
    - put rx/tx packet routine to another thread
*/

bool network_manager_init()
{
    net_devices = heap_kernel_alloc(sizeof(kvector), 0);
    kvector_init(net_devices);
    //Initialize all NIC drivers
    net_device_t dev;
    dev.receive_packet = &network_manager_receive_packet;

    //If RTL8139 is present
    if (rtl8139_init(&dev))
    {
        net_device_t *tmp = heap_kernel_alloc(sizeof(net_device_t), 0);
        memcpy(tmp, &dev, sizeof(net_device_t));
        tmp->rx_queue = heap_kernel_alloc(sizeof(kvector), 0);
        tmp->tx_queue = heap_kernel_alloc(sizeof(kvector), 0);
        kvector_init(tmp->rx_queue);
        kvector_init(tmp->tx_queue);
        kvector_add(net_devices, tmp);
        //Hardcode IPv4 address
        __set_ipv4_addr(tmp->ipv4_address, 192,160,1,120);
        __network_manager_print_device_info(tmp);
    }

    //If VirtIO is present
    if (virtio_nic_init(&dev))
    {
        net_device_t *tmp = heap_kernel_alloc(sizeof(net_device_t), 0);
        memcpy(tmp, &dev, sizeof(net_device_t));
        tmp->rx_queue = heap_kernel_alloc(sizeof(kvector), 0);
        tmp->tx_queue = heap_kernel_alloc(sizeof(kvector), 0);
        kvector_init(tmp->rx_queue);
        kvector_init(tmp->tx_queue);
        kvector_add(net_devices, tmp);
        //Hardcode IPv4 address
        __set_ipv4_addr(tmp->ipv4_address, 192,160,1,120);
        __network_manager_print_device_info(tmp);
    }

    // net_packet_t* test_packet = heap_kernel_alloc(sizeof(net_packet_t),0);
    // memcpy(test_packet->device_mac,((net_device_t*)(net_devices->data[0]))->mac_address,MAC_ADDRESS_SIZE);
    // char* data = heap_kernel_alloc(16,0);
    // data = "Hello!";
    // test_packet->packet_data = data;
    // test_packet->packet_length = 16;
    // network_manager_send_packet(test_packet);

    return true;
}

void network_manager_send_packet(net_packet_t *packet)
{
    for (uint8_t i = 0; i < net_devices->count; i++)
    {
        //Add packet to device RX queue
        if (__compare_mac_address(((net_device_t *)(net_devices->data[i]))->mac_address, packet->device_mac))
        {
            //TODO: ADD BUFFERING
            //buffers will be necessary, when we use multithreading
            //kvector_add(((net_device_t *)(net_devices->data[i]))->rx_queue, packet);
            //when all processes are sequential, there is no need to use buffers
            //just send packet
            ((net_device_t *)(net_devices->data[i]))->send_packet(packet);
            bytes_sent_count += packet->packet_length;
            break;
        }
    }
}

void network_manager_receive_packet(net_packet_t *packet)
{
    for (uint8_t i = 0; i < net_devices->count; i++)
    {
        //Add packet to device RX queue
        if (__compare_mac_address(((net_device_t *)(net_devices->data[i]))->mac_address, packet->device_mac))
        {
            //TODO: ADD BUFFERING
            //buffers will be necessary, when we use multithreading
            //kvector_add(((net_device_t *)(net_devices->data[i]))->rx_queue, packet);
            //just process an packet
            network_manager_process_packet(packet);
            bytes_received_count += packet->packet_length;
            break;
        }
    }

    //packets are on heap, so free memory
    heap_kernel_dealloc(packet->packet_data);
    heap_kernel_dealloc(packet);
}

void network_manager_process_packet(net_packet_t *packet)
{
    ethernet_frame_t frame = *(ethernet_frame_t *)packet->packet_data;
    frame.type = __uint16_flip(frame.type);
    //last is offset of data ptr;
    void *data_ptr = packet->packet_data + sizeof(ethernet_frame_t) - sizeof(uint8_t *);
    
    switch (frame.type)
    {
    case ARP_PROTOCOL_TYPE:
        logger_log_info("ARP");
        arp_process_packet((arp_packet_t *)data_ptr, packet->device_mac);
        break;
    case IPv4_PROTOCOL_TYPE:
        logger_log_info("IP");
        ipv4_process_packet((ipv4_packet_t *)data_ptr, packet->device_mac);
        break;
    };
}

uint64_t network_manager_bytes_sent()
{
    return bytes_sent_count;
}

uint64_t network_manager_bytes_received()
{
    return bytes_received_count;
}

uint8_t *network_manager_verify_ipv4_address(uint8_t *ipv4_address)
{
    for (uint8_t i = 0; i < net_devices->count; i++)
    {
        net_device_t *dev = *(net_devices->data + i);
        uint8_t flag = 1;

        for (uint8_t octet = 0; octet < IPv4_ADDRESS_LENGTH; octet++)
        {
            if (dev->ipv4_address[octet] != ipv4_address[octet])
            {
                flag = 0;
                break;
            }
        }

        //Found NIC with valid IPv4
        if (flag)
        {
            uint8_t *ret = heap_kernel_alloc(sizeof(uint8_t) * MAC_ADDRESS_LENGTH, 0);
            memcpy(ret, dev->mac_address, sizeof(uint8_t) * MAC_ADDRESS_LENGTH);
            return ret;
        }
    }

    return 0;
}

bool network_manager_send_ethernet_frame(ethernet_frame_t *frame, uint32_t data_len)
{
    void *data = heap_kernel_alloc(sizeof(ethernet_frame_t) + data_len, 0);

    net_packet_t packet;
    packet.packet_length = data_len + sizeof(ethernet_frame_t) - sizeof(void *);
    memcpy(packet.device_mac, frame->src_mac_addr, MAC_ADDRESS_SIZE);
    packet.packet_data = data;

    uint8_t offset = sizeof(ethernet_frame_t) - sizeof(void *);
    memcpy(data, frame, offset);
    memcpy(data + offset, frame->data, data_len);

    network_manager_send_packet(&packet);
    heap_kernel_dealloc(data);
    return true;
}

ethernet_frame_t *network_manager_make_frame(uint8_t *src_hw, uint8_t *dst_hw, uint16_t type)
{
    ethernet_frame_t *frame = heap_kernel_alloc(sizeof(ethernet_frame_t), 0);

    memcpy(frame->src_mac_addr, src_hw, MAC_ADDRESS_SIZE);
    memcpy(frame->dst_mac_addr, dst_hw, MAC_ADDRESS_SIZE);

    frame->type = __uint16_flip(type);

    return frame;
}

net_device_t *network_manager_get_nic()
{
    if (net_devices == 0)
        return 0;

    if (net_devices->count == 0)
        return 0;

    return net_devices->data[0];
}

void __network_manager_print_device_info(net_device_t *device)
{
    logger_log_ok(device->device_name);

    char* logInfo[27];
    kernel_sprintf(logInfo, "MAC: %02x:%02x:%02x:%02x:%02x:%02x",
                    device->mac_address[0],
                    device->mac_address[1],
                    device->mac_address[2],
                    device->mac_address[3],
                    device->mac_address[4],
                    device->mac_address[5]);
    logger_log_info(logInfo);
}
