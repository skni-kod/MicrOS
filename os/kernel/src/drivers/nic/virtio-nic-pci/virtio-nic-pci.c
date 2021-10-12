/*
    @JakubPrzystasz
    Created: 28.07.2021
    Modify: 28.07.2021
*/
#include "virtio-nic-pci.h"

pci_device pci_virtio_nic_device = {0};
virtio_nic_dev virtio_nic = {0};

virtq *receiveQueue;
virtq *transmitQueue;

uint32_t VNet_Read_Register(uint16_t reg)
{
    // if 4-byte register
    if (reg < REG_QUEUE_SIZE)
    {
        return io_in_long(virtio_nic.io_base + reg);
    }
    else
    {
        // if 2-byte register
        if (reg <= REG_QUEUE_NOTIFY)
            return io_in_word(virtio_nic.io_base + reg);
        else // 1-byte register
            return io_in_byte(virtio_nic.io_base + reg);
    }
}

void VNet_Write_Register(uint16_t reg, uint32_t data)
{
    // if 4-byte register
    if (reg < REG_QUEUE_SIZE)
    {
        io_out_long(virtio_nic.io_base + reg, data);
    }
    else
    {
        // if 2-byte register
        if (reg <= REG_QUEUE_NOTIFY)
            io_out_word(virtio_nic.io_base + reg, (uint16_t)data);
        else // 1-byte register
            io_out_byte(virtio_nic.io_base + reg, (uint8_t)data);
    }
}

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

        if (dev->vendor_id == VIRTIO_NET_DEVICE_VENDOR_ID &&
            dev->device_id >= VIRTIO_NET_DEVICE_ID_BEGIN &&
            dev->device_id <= VIRTIO_NET_DEVICE_ID_END &&
            dev->subsystem_id == VIRTIO_NET_DEVICE_SUBSYSTEM_ID)
        {
            pci_virtio_nic_device = *dev;
            break;
        }
    }

    //Device not present in PCI bus
    if (pci_virtio_nic_device.vendor_id == 0)
        return false;

    //Now setup registers, and memory
    virtio_nic.bar_type = pci_virtio_nic_device.base_addres_0 & (0x1);
    virtio_nic.io_base = pci_virtio_nic_device.base_addres_0 & (~0x3);
    virtio_nic.mem_base = pci_virtio_nic_device.base_addres_0 & (~0xf);

    // Reset the virtio-network device
    VNet_Write_Register(REG_DEVICE_STATUS, STATUS_RESET_DEVICE);

    // Set the acknowledge status bit
    VNet_Write_Register(REG_DEVICE_STATUS, STATUS_DEVICE_ACKNOWLEDGED);

    // Set the driver status bit
    VNet_Write_Register(REG_DEVICE_STATUS, STATUS_DEVICE_ACKNOWLEDGED | STATUS_DRIVER_LOADED);

    // Read the feature bits
    uint32_t features = VNet_Read_Register(REG_DEVICE_FEATURES);

    // Make sure the features we need are supported
    if ((features & REQUIRED_FEATURES) != REQUIRED_FEATURES)
    {
        logger_log_warning("Required features are not supported by device. Aborting");
        VNet_Write_Register(REG_DEVICE_STATUS, STATUS_DEVICE_ERROR);
        return;
    }

    // Tell the device what features we'll be using
    VNet_Write_Register(REG_GUEST_FEATURES, REQUIRED_FEATURES);

    //PCI bus mastering
    uint32_t pci_bus_config = pci_io_in(&pci_virtio_nic_device, 1);
    pci_bus_config |= 0x4;
    pci_io_out(&pci_virtio_nic_device, 1, pci_bus_config);

    /*
        // Init virtqueues (see 4.1.5.1.3 of virtio-v1.0-cs04.pdf)
    // Since we don't negotiate VIRTIO_NET_F_MQ, we can expect 3 virtqueues: receive, transmit, and control
    */
    receiveQueue = heap_kernel_alloc(sizeof(virtq), 0);
    transmitQueue = heap_kernel_alloc(sizeof(virtq), 0);

    VirtIO_Net_Init_Virtqueue(receiveQueue, 0);
    VirtIO_Net_Init_Virtqueue(transmitQueue, 1);
    VirtIO_Net_SetupReceiveBuffers();
    receiveQueue->deviceArea->flags = 0;

    //Enable IRQ
    uint32_t irq_num = pci_virtio_nic_device.interrupt_line;
    pic_enable_irq(irq_num);
    idt_attach_interrupt_handler(irq_num, virtio_nic_irq_handler);

    //Get mac address
    for (uint8_t i = 0; i < 6; i++)
        virtio_nic.mac_addr[i] = io_in_byte(virtio_nic.io_base + 0x14 + i);

    net_dev->device_name = heap_kernel_alloc(strlen(VIRTIO_NET_DEVICE_NAME) + 1, 0);
    strcpy(net_dev->device_name, VIRTIO_NET_DEVICE_NAME);
    memcpy(net_dev->mac_address, virtio_nic.mac_addr, sizeof(uint8_t) * 6);


    // Tell the device it's initialized
    VNet_Write_Register(REG_DEVICE_STATUS, STATUS_DRIVER_READY);

    // Remind the device that it has receive buffers. Hey VirtualBox! Why aren't you using these?
    VNet_Write_Register(REG_QUEUE_NOTIFY, VIRTIO_NET_RECEIVE_QUEUE_INDEX);

    return true;
}

bool virtio_nic_irq_handler()
{
    logger_log_info("VirtIO NET: DUPA");

    return true;
}

void VirtIO_Net_Init_Virtqueue(virtq *virtqueue, uint16_t queueIndex)
{
    int16_t queueSize = -1;

    // access the current queue
    VNet_Write_Register(REG_QUEUE_SELECT, queueIndex);

    // get the size of the current queue
    queueSize = (uint16_t)VNet_Read_Register(REG_QUEUE_SIZE);

    if (!queueSize)
        return;

    kernel_sprintf(str, "queue %d has %d elements", queueIndex, queueSize);
    logger_log_info(str);

    // Allocate and initialize the queue
    VirtIO_Allocate_Virtqueue(virtqueue, queueSize);

    kernel_sprintf(str, "queue %d: 0x%u", queueIndex, virtqueue->descriptors);
    logger_log_info(str);

    kernel_sprintf(str, "queue size: %x", virtqueue->elements);
    logger_log_info(str);

    kernel_sprintf(str, "driverArea: 0x%x", virtqueue->driverArea);
    logger_log_info(str);

    kernel_sprintf(str, "deviceArea: 0x%x", virtqueue->deviceArea);
    logger_log_info(str);

    uint32_t addr = (uint32_t)((uint32_t)virtqueue->descriptors - DMA_ADDRESS_OFFSET) / 4096;

    VNet_Write_Register(REG_QUEUE_ADDRESS, addr);
}

void VirtIO_Net_SetupReceiveBuffers()
{
    const uint16_t bufferSize = 1526; // as per virtio specs

    // Allocate and add 16 buffers to receive queue
    for (uint16_t i = 0; i < 16; ++i)
    {
        uint32_t buffer = ((uint32_t)heap_kernel_alloc(bufferSize, 0) - DMA_ADDRESS_OFFSET);

        // Add buffer to the descriptor table
        receiveQueue->descriptors[i].address = (uint64_t)buffer;
        receiveQueue->descriptors[i].flags = VIRTQ_DESC_F_DEVICE_WRITE_ONLY;
        receiveQueue->descriptors[i].length = bufferSize;
        receiveQueue->descriptors[i].next = 0;

        // Add index of descriptor to the driver ring
        receiveQueue->driverArea->ringBuffer[i] = i;
    }

    // Update next available index
    receiveQueue->driverArea->index = 16;
    VNet_Write_Register(REG_QUEUE_NOTIFY, VIRTIO_NET_RECEIVE_QUEUE_INDEX);
}

void VirtIO_Net_SendPacket(net_packet_t *packet)
{
    VNet_Write_Register(REG_QUEUE_NOTIFY, VIRTIO_NET_TRANSMIT_QUEUE_INDEX);
}