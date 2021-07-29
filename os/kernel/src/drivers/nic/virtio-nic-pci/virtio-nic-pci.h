/*
    @JakubPrzystasz
    Created: 28.07.2021
    Modify: 28.07.2021
*/
#ifndef VIRTIO_NET_PCI_H
#define VIRTIO_NET_PCI_H

#include "../../pci/pci.h"
#include "../../../assembly/io.h"
#include "../../../logger/logger.h"
#include "../../../cpu/idt/idt.h"
#include "../../serial/serial.h"
#include "../../../network/network_definitions.h"
#include "../../../klibrary/kvector.h"
#include <stdlib.h>

#define VIRTIO_NIC_DEVICE_VENDOR_ID 0x1AF4
#define VIRTIO_NIC_DEVICE_CLASS_ID 0x2
#define VIRTIO_NIC_DEVICE_ID_BEGIN 0x1000
#define VIRTIO_NIC_DEVICE_ID_END 0x103F
#define VIRTIO_NIC_DEVICE_NAME "VirtIO NIC"

typedef struct virtio_nic_dev
{
    uint8_t bar_type;
    uint16_t io_base;
    uint32_t mem_base;
    uint8_t mac_addr[6];
    uint8_t status;
} virtio_nic_dev_t;

bool virtio_nic_init(net_device_t *net_dev);


#endif