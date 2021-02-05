#include "network_manager.h"

kvector *net_devices;

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
        __network_manager_print_device_info(tmp);
        kvector_add(net_devices, tmp);
    }

    return true;
}

void network_manager_receive_packet(net_packet_t *packet)
{
    for (uint8_t i = 0; i < net_devices->count; i++)
    {
        //Add packet to device RX queue
        if (__network_manager_compare_mac_address(((net_device_t *)(net_devices->data[i]))->mac_address, packet->device_mac))
        {
            kvector_add(((net_device_t *)(net_devices->data[i]))->rx_queue, packet);
            break;
        }
    }
}

void __network_manager_print_device_info(net_device_t *device)
{
    logger_log_ok(device->device_name);

    char logInfo[27] = "MAC: __:__:__:__:__:__";
    char numberBuffer[3];
    for (uint8_t i = 0; i < 6; i++)
    {
        itoa(device->mac_address[i], numberBuffer, 16);
        //Add leading zero
        if (numberBuffer[1] == 0)
        {
            numberBuffer[1] = numberBuffer[0];
            numberBuffer[0] = '0';
        }
        //Copy just 2 characters, we dont need ending 0
        memcpy(logInfo + (5 + i * 3), numberBuffer, 2);
    }

    logger_log_info(logInfo);
}

bool __network_manager_compare_mac_address(uint8_t *first, uint8_t *second)
{
    for (char i = 0; i < 6; i++)
    {
        if (*(first + i) != *(second + i))
            return false;
    }

    return true;
}