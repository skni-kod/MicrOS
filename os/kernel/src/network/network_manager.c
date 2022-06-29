/*
    @JakubPrzystasz
    Created: 06.02.2021
    Modified: 27.06.2022
    Ref: https://stackoverflow.com/questions/31446777/difference-between-packets-and-frames
*/

#include "network_manager.h"

kvector *net_devices;
// TODO: these functions have to be dynamically loaded from drivers file
bool (*drivers[])(net_device_t *device) = {rtl8139_init, virtio_nic_init};
uint8_t drivers_count = 2;

bool network_manager_init()
{
    net_devices = heap_kernel_alloc(sizeof(kvector), 0);
    kvector_init(net_devices);

    // Initialize all NIC drivers
    for (uint8_t i = 0; i < drivers_count; i++)
    {
        net_device_t *dev = heap_kernel_alloc(sizeof(net_device_t), 0);

        if (__network_manager_set_net_device(dev) && drivers[i](dev))
        {
            // TODO: Buffering, hardcoding ip??
            __set_ipv4_addr(dev->configuration->ipv4_address, 10, 0, 2, 15);
            // set nic to send/receive mode
            dev->configuration->mode = 0x3;
            kvector_add(net_devices, dev);
            __network_manager_print_device_info(dev);
        }
        else
        {
            heap_kernel_dealloc(dev->configuration);
            heap_kernel_dealloc(dev);
        }
    }

    return true;
}

nic_data_t *network_manager_get_receive_buffer(net_device_t *device, uint32_t size)
{
    if (0 != device && 0 < size)
    {
        nic_data_t *data = (nic_data_t *)heap_kernel_alloc(sizeof(nic_data_t), 0);
        data->length = size;
        data->device = device;
        data->frame = (uint8_t *)heap_kernel_alloc(size, 0);
        return data;
    }
    else
        return 0;
}

void network_manager_send_data(nic_data_t *data)
{
    //TODO: Do something with card config (TX_RX)
    data->device->dpi.send(data);
    ++data->device->frames_sent;
    data->device->bytes_sent += data->length;

    heap_kernel_dealloc(data->frame);
    heap_kernel_dealloc(data);
}

void network_manager_receive_data(nic_data_t *data)
{

    //TODO: Do something with card config (TX_RX)
    switch (data->device->configuration->frame_type)
    {
    case ethernet:
    default:
        ethernet_process_frame(data);
        break;
    }

    ++data->device->frames_received;
    data->device->bytes_received += data->length;

    heap_kernel_dealloc(data->frame);
    heap_kernel_dealloc(data);
}

net_device_t *network_manager_get_nic()
{
    if (net_devices == 0)
        return 0;

    if (net_devices->count == 0)
        return 0;

    return net_devices->data[0];
}

net_device_t *network_manager_get_nic_by_ipv4(uint8_t *ipv4_address)
{
    for (uint8_t i = 0; i < net_devices->count; i++)
    {
        net_device_t *dev = *(net_devices->data + i);
        if (__ipv4_compare_address(ipv4_address, dev->configuration->ipv4_address))
            return dev;
    }

    return 0;
}

net_device_t *network_manager_get_nic_by_mac(uint8_t *mac)
{
    for (uint8_t i = 0; i < net_devices->count; i++)
    {
        net_device_t *dev = *(net_devices->data + i);
        if (__arp_compare_mac_address(mac, dev->configuration->mac_address))
            return dev;
    }
    return 0;
}

void __network_manager_print_device_info(net_device_t *device)
{
    char logInfo[27] = "";

    logger_log_ok(device->device_name);

    kernel_sprintf(logInfo, "MAC: %02x:%02x:%02x:%02x:%02x:%02x",
                   device->configuration->mac_address[0],
                   device->configuration->mac_address[1],
                   device->configuration->mac_address[2],
                   device->configuration->mac_address[3],
                   device->configuration->mac_address[4],
                   device->configuration->mac_address[5]);

    logger_log_info(logInfo);
}

bool __network_manager_set_net_device(net_device_t *device)
{
    if (!device)
        return 0;
    memset(device, 0, sizeof(net_device_t));

    device->dpi.receive = &network_manager_receive_data;
    device->dpi.get_receive_buffer = &network_manager_get_receive_buffer;

    device->configuration = heap_kernel_alloc(sizeof(device_configuration_t), 0);

    device->configuration->arp_entries = heap_kernel_alloc(sizeof(kvector), 0);
    kvector_init(device->configuration->arp_entries);

    if (!device->configuration)
        return 0;
    memset(device->configuration, 0, sizeof(device_configuration_t));

    return 1;
}
