/*
    @JakubPrzystasz
    Created: 06.02.2021
    Modified: 27.06.2022
*/

#include "network_manager.h"

kvector *net_devices;
bool (*drivers[])(net_device_t *device) = {rtl8139_init, virtio_nic_init};
uint8_t drivers_count = 2;

bool network_manager_init()
{   

    net_devices = heap_kernel_alloc(sizeof(kvector), 0);
    kvector_init(net_devices);

    ipv4_init();

    // Initialize all NIC drivers
    for (uint8_t i = 0; i < drivers_count; i++)
    {
        net_device_t *dev = heap_kernel_alloc(sizeof(net_device_t), 0);
        if (__network_manager_set_net_device(dev) && drivers[i](dev))
        {
            // TODO: Buffering, hardcoding ip??
            dev->interface->ipv4.oct_a = 192;
            dev->interface->ipv4.oct_b = 168;
            dev->interface->ipv4.oct_c = 100;
            dev->interface->ipv4.oct_d = 151 + net_devices->count;
            kvector_add(net_devices, dev);
            __network_manager_print_device_info(dev);
        }
        else
        {
            heap_kernel_dealloc(dev->interface);
            heap_kernel_dealloc(dev);
        }
    }

    // when all devices are up, lets initalize them!
    for (uint32_t devices = 0; devices < net_devices->count; devices++)
    {
        net_device_t *dev = (net_device_t *)net_devices->data[devices];
        dev->interface->mode.value = 0x3;
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
        data->keep = false;
        return data;
    }
    else
        return 0;
}

void network_manager_send_data(nic_data_t *data)
{
    if (data->device->interface->mode.send)
    {
        data->device->dpi.send(data);
        ++data->device->frames_sent;
        data->device->bytes_sent += data->length;
    }

    heap_kernel_dealloc(data->frame);
    heap_kernel_dealloc(data);
}

void network_manager_receive_data(nic_data_t *data)
{

    if (data->device->interface->mode.receive)
    {
        ethernet_process_frame(data);
        ++data->device->frames_received;
        data->device->bytes_received += data->length;
    }
    if (!(data->keep))
    {
        heap_kernel_dealloc(data->frame);
        heap_kernel_dealloc(data);
    }
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
    char logInfo[27] = "";

    logger_log_ok(device->device_name);

    kernel_sprintf(logInfo, "MAC: %02x:%02x:%02x:%02x:%02x:%02x",
                   device->interface->mac.octet_a,
                   device->interface->mac.octet_b,
                   device->interface->mac.octet_c,
                   device->interface->mac.octet_d,
                   device->interface->mac.octet_e,
                   device->interface->mac.octet_f);

    logger_log_info(logInfo);
}

bool __network_manager_set_net_device(net_device_t *device)
{
    if (!device)
        return 0;
    memset(device, 0, sizeof(net_device_t));

    //TODO:BUFFERING
    device->dpi.receive = &network_manager_receive_data;
    device->dpi.get_receive_buffer = &network_manager_get_receive_buffer;

    device->dpi.get_send_buffer = &network_manager_get_receive_buffer;


    device->interface = heap_kernel_alloc(sizeof(net_interface_t), 0);
    if (!device->interface)
        return 0;

    memset(device->interface, 0, sizeof(net_interface_t));

    device->interface->arp_entries = heap_kernel_alloc(sizeof(kvector), 0);
    if (!device->interface->arp_entries)
        return 0;
    kvector_init(device->interface->arp_entries);

    // TODO: get mtu from driver
    device->interface->mtu = 1500;

    return 1;
}
