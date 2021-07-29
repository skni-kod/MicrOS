/*
    @JakubPrzystasz
    Created: 28.07.2021
    Modify: 28.07.2021
*/
#include "virtio-nic-pci.h"

pci_device pci_virtio_nic_device = {0};
virtio_nic_dev_t virtio_nic = {0};

bool virtio_nic_init(net_device_t *net_dev)
{

    //Prevent for re-init
    if (pci_virtio_nic_device.vendor_id != 0)
        return false;

    // First get the network device using PCI
    // PCI must be initialized!

    for (int i = 0; i < pci_get_number_of_devices(); i++)
    {
        pci_device *dev = pci_get_device(i);

        if (dev->vendor_id == VIRTIO_NIC_DEVICE_VENDOR_ID && dev->device_id >= VIRTIO_NIC_DEVICE_ID_BEGIN &&  dev->device_id <= VIRTIO_NIC_DEVICE_ID_END)
        {
            if (dev->class_code == VIRTIO_NIC_DEVICE_CLASS_ID)
            {
                pci_virtio_nic_device = *dev;
                break;
            }
        }
    }

    //Device not present in PCI bus
    if (pci_virtio_nic_device.vendor_id == 0)
        return false;

    //Now setup registers, and memory
    virtio_nic.bar_type = pci_virtio_nic_device.base_addres_0 & (0x1);
    virtio_nic.io_base = pci_virtio_nic_device.base_addres_0 & (~0x3);
    virtio_nic.mem_base = pci_virtio_nic_device.base_addres_0 & (~0xf);

    //PCI bus mastering
    uint32_t pci_bus_config = pci_io_in(&pci_virtio_nic_device, 1);
    pci_bus_config |= 0x4;
    pci_io_out(&pci_virtio_nic_device, 1, pci_bus_config);

    //Get mac address
    for (uint8_t i = 0; i < 6; i++)
        virtio_nic.mac_addr[i] = io_in_byte(virtio_nic.io_base + 0x14 + i);

    virtio_nic.status = io_in_byte(virtio_nic.io_base + 0x1A);

    net_dev->device_name = heap_kernel_alloc(strlen(VIRTIO_NIC_DEVICE_NAME) + 1, 0);
    strcpy(net_dev->device_name, VIRTIO_NIC_DEVICE_NAME);

    memcpy(net_dev->mac_address, virtio_nic.mac_addr, sizeof(uint8_t) * 6);
    //net_dev->send_packet = &rtl8139_send_packet;
    //net_dev->sent_count = &rtl8139_get_sent_count;
    //net_dev->received_count = &rtl8139_get_received_count;

    return true;
}