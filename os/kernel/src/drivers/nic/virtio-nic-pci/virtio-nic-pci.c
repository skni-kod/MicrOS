/*
    @JakubPrzystasz
    Created: 28.07.2021
    Modify: 28.07.2021
*/
#include "virtio-nic-pci.h"

pci_device pci_virtio_nic_device = {0};

bool virtio_nic_init(net_device_t *net_dev){
    
     //Prevent for re-init
    if (pci_virtio_nic_device.vendor_id != 0)
        return false;

    // First get the network device using PCI
    // PCI must be initialized!

    for (int i = 0; i < pci_get_number_of_devices(); i++)
    {
        pci_device *dev = pci_get_device(i);
        if (dev->vendor_id == DEVICE_VENDOR_ID)
        {
            if (dev->class_code == DEVICE_CLASS_ID)
            {
                pci_virtio_nic_device = *dev;
                break;
            }
        }
    }
    
    //Device not present in PCI bus
    if (pci_virtio_nic_device.vendor_id == 0)
        return false;


    net_dev->device_name = heap_kernel_alloc(strlen(DEVICE_NAME) + 1, 0);
    strcpy(net_dev->device_name, DEVICE_NAME);

    return true;
}