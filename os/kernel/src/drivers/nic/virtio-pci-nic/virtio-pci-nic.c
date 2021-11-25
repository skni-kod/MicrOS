/*
    @JakubPrzystasz
    Created: 28.07.2021
    Last modified: 25.11.2021
*/
#include "virtio-pci-nic.h"

pci_device pci_virtio_nic_device = {0};
virtio_nic_dev virtio_nic = {0};

virtq *receiveQueue;
virtq *transmitQueue;

void (*receive_packet)(net_packet_t *);

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
    VNet_Write_Register(REG_DEVICE_STATUS, STATUS_DEVICE_ACKNOWLEDGE);

    // Set the driver status bit
    VNet_Write_Register(REG_DEVICE_STATUS, STATUS_DEVICE_ACKNOWLEDGE | STATUS_DRIVER_LOADED);

    // Read the feature bits
    uint32_t features = VNet_Read_Register(REG_DEVICE_FEATURES);

    // Make sure the features we need are supported
    if ((features & REQUIRED_FEATURES) != REQUIRED_FEATURES)
    {
        VNet_Write_Register(REG_DEVICE_STATUS, STATUS_DEVICE_NEEDS_RESET);
        return;
    }

    // Tell the device what features we'll be using
    VNet_Write_Register(REG_DRIVER_FEATURES, REQUIRED_FEATURES);

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
    receiveQueue->device_area->flags = 0;

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
    VNet_Write_Register(REG_DEVICE_STATUS, STATUS_DRIVER_OK);

    // Remind the device that it has receive buffers. Hey VirtualBox! Why aren't you using these?
    VNet_Write_Register(REG_QUEUE_NOTIFY, VIRTIO_NET_RECEIVE_QUEUE_INDEX);

    receive_packet = net_dev->receive_packet;

    // net_packet_t *packet = heap_kernel_alloc(sizeof(net_packet_t), 0);
    // packet->packet_length = 16;
    // packet->packet_data = heap_kernel_alloc(16, 0);
    // char str[] = "No i dupa";
    // strcpy(packet->packet_data, str);

    // VirtIO_Net_SendPacket(packet);

    return true;
}

bool virtio_nic_irq_handler()
{
    // Check for used queues
    if (VNet_Read_Register(REG_ISR_STATUS) & VIRTIO_ISR_VIRTQ_USED)
    {
        //terminal_writestring("Virtq used\n");

        // see if the transmit queue has been used
        // while (transmitQueue.device_area->index != transmitQueue.lastdevice_areaIndex)
        // {
        //     if (debugLevel)
        //         terminal_writestring("Transmit success\n");
        //     // free used buffers
        //     uint16_t deviceRingIndex = transmitQueue.lastdevice_areaIndex % transmitQueue.elements;
        //     uint16_t descIndex = (uint16_t)transmitQueue.device_area->ringBuffer[deviceRingIndex].index;
        //     descIndex %= transmitQueue.elements;

        //     if(debugLevel)
        //         kprintf("descIndex: %d\n", descIndex);

        //     dbg_release((void *)(uint32_t)transmitQueue.descriptor_area[descIndex].address);

        //     transmitQueue.descriptor_area[descIndex].address = 0xFFFFFFFFffffffff;
        //     transmitQueue.descriptor_area[descIndex].length = 0;    // might not be necessary

        //     while (transmitQueue.descriptor_area[descIndex].next)
        //     {
        //         transmitQueue.descriptor_area[descIndex++].next = 0;
        //         descIndex %= transmitQueue.elements;
        //         if (!(uint32_t)transmitQueue.descriptor_area[descIndex].address)
        //             kprintf("That's bad!\n");
        //         dbg_release((void *)(uint32_t)transmitQueue.descriptor_area[descIndex].address);
        //         transmitQueue.descriptor_area[descIndex].address = 0xFFFFFFFFffffffff;
        //         transmitQueue.descriptor_area[descIndex].length = 0;    // might not be necessary
        //     }
        //     transmitQueue.lastdevice_areaIndex++;
        // }

        // see if the receive queue has been used
        if (receiveQueue->device_area->index != receiveQueue->last_device_index)
        {
            VirtIO_Net_ReceivePacket();
        }
    }

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

    // Allocate and initialize the queue
    virtio_setup_queue(virtqueue, queueSize);

    // kernel_sprintf(str, "queue %d has %d elements", queueIndex, queueSize);
    // logger_log_info(str);

    // kernel_sprintf(str, "queue %d: 0x%u", queueIndex, virtqueue->descriptor_area);
    // logger_log_info(str);

    // kernel_sprintf(str, "queue size: %x", virtqueue->elements);
    // logger_log_info(str);

    // kernel_sprintf(str, "driver_area: 0x%x", virtqueue->driver_area);
    // logger_log_info(str);

    // kernel_sprintf(str, "device_area: 0x%x", virtqueue->device_area);
    // logger_log_info(str);

    uint32_t addr = (uint32_t)((uint32_t)virtqueue->descriptor_area - DMA_ADDRESS_OFFSET) / 4096;

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
        receiveQueue->descriptor_area[i].address = (uint64_t)buffer;
        receiveQueue->descriptor_area[i].flags = VIRTQ_DESC_F_DEVICE_WRITE_ONLY;
        receiveQueue->descriptor_area[i].length = bufferSize;
        receiveQueue->descriptor_area[i].next = 0;

        // Add index of descriptor to the driver ring
        receiveQueue->driver_area->ring_buffer[i] = i;
    }

    // Update next available index
    receiveQueue->driver_area->index = 16;
    VNet_Write_Register(REG_QUEUE_NOTIFY, VIRTIO_NET_RECEIVE_QUEUE_INDEX);
}

void VirtIO_Net_SendPacket(net_packet_t *packet)
{

    uint16_t bufferSize = sizeof(virtio_nic_net_header);

    // Allocate a buffer for the header
    virtio_nic_net_header *netBuffer = heap_kernel_alloc(bufferSize, 0);

    // Set parameters of netBuffer
    memset(netBuffer, 0, sizeof(virtio_nic_net_header));

    // Get indices for the next two descriptor_area
    uint16_t descIndex = transmitQueue->next_descriptor % transmitQueue->size;
    uint16_t descIndex2 = (descIndex + 1) % transmitQueue->size;
    transmitQueue->next_descriptor += 2;

    // Get index for the next entry into the available-ring
    uint16_t index = transmitQueue->driver_area->index % transmitQueue->size;

    // fill descriptor with net header
    transmitQueue->descriptor_area[descIndex].address = (uint64_t)netBuffer - DMA_ADDRESS_OFFSET;
    if (transmitQueue->descriptor_area[descIndex].address == 0xdeadbeef || transmitQueue->descriptor_area[descIndex].address == 0xdeadbeefdeadbeef)
        return;
    transmitQueue->descriptor_area[descIndex].flags = VIRTQ_DESC_F_NEXT;
    transmitQueue->descriptor_area[descIndex].length = bufferSize;
    transmitQueue->descriptor_area[descIndex].next = descIndex2;

    // copy packet to new buffer, because packetBuffer won't be a physical address
    uint8_t *packetBuffer = heap_kernel_alloc(packet->packet_length, 0);

    memcpy(packetBuffer, packet->packet_data, packet->packet_length);
    // (TODO: malloc returns identity-mapped addresses for now but later we'll need a function to convert virtual to physical)

    // fill descriptor with ethernet packet
    transmitQueue->descriptor_area[descIndex2].address = (uint64_t)(packetBuffer - DMA_ADDRESS_OFFSET);
    if (transmitQueue->descriptor_area[descIndex2].address == 0xdeadbeef || transmitQueue->descriptor_area[descIndex].address == 0xdeadbeefdeadbeef)
        return;
    transmitQueue->descriptor_area[descIndex2].flags = 0;
    transmitQueue->descriptor_area[descIndex2].length = packet->packet_length;
    transmitQueue->descriptor_area[descIndex2].next = 0;

    // Add descriptor chain to the available ring
    transmitQueue->driver_area->ring_buffer[index] = descIndex;

    // Increase available ring index and notify the device
    transmitQueue->driver_area->index++;

    VNet_Write_Register(REG_QUEUE_NOTIFY, VIRTIO_NET_TRANSMIT_QUEUE_INDEX);
}

void VirtIO_Net_ReceivePacket()
{
    while (receiveQueue->device_area->index != receiveQueue->last_device_index)
    {
        // Get the index of the current descriptor from the device's ring buffer
        uint16_t ringBufferIndex = receiveQueue->last_device_index % receiveQueue->size;
        uint16_t descIndex = (uint16_t)receiveQueue->device_area->ring_buffer[ringBufferIndex].index;

        // Get pointer to the beginning of the buffer
        uint32_t *rxBegin = (uint32_t *)((uint32_t)receiveQueue->descriptor_area[descIndex % receiveQueue->size].address + DMA_ADDRESS_OFFSET);

        // See if the buffer spans multiple descriptor_area
        int buffers = ((virtio_nic_net_header *)rxBegin)->buffers_count;

        // TODO:
        // if (buffers > 1)
        // {
        //     uint32_t bufferSize = 0;

        //     // determine length of entire buffer that spans the descriptor chain
        //     for (int i = 0; i < buffers; ++i)
        //     {
        //         bufferSize += receiveQueue->descriptor_area[(descIndex + i) % receiveQueue->elements].length;
        //     }

        //     rxBegin = (heap_kernel_alloc(bufferSize, 0)) - DMA_ADDRESS_OFFSET;
        //     if (!rxBegin)
        //     {
        //         logger_log_warning("Not enough memory to allocate rxBegin\n");
        //         return; // TODO: Is this the best way to handle this
        //     }

        //     // Copy buffer pieces to single buffer
        //     uint32_t offset = 0;
        //     for (int i = 0; i < buffers; ++i)
        //     {
        //         memcpy((void *)((uint32_t)rxBegin + offset),
        //                (void *)(uint32_t)receiveQueue->descriptor_area[(descIndex + i) % receiveQueue->elements].address,
        //                receiveQueue->descriptor_area[(descIndex + i) % receiveQueue->elements].length);
        //         offset += receiveQueue->descriptor_area[(descIndex + i) % receiveQueue->elements].length;
        //     }
        // }

        // Skip over virtio_net_hdr to get a pointer to the packet
        uint32_t *packet = ((uint32_t)rxBegin + sizeof(virtio_nic_net_header));

        //Get packet length by skipping packet header
        uint32_t packet_length = receiveQueue->descriptor_area[descIndex % receiveQueue->size].length;

        //Copy received data to heap
        void *packet_data = heap_kernel_alloc(packet_length, 0);
        memcpy(packet_data, packet, packet_length);

        //Add packet to packets queue
        net_packet_t *out = heap_kernel_alloc(sizeof(net_packet_t), 0);
        out->packet_data = packet_data;
        out->packet_length = packet_length;
        VirtIO_Net_GetMAC(out->device_mac);

        (*receive_packet)(out);

        // Place the used descriptor indices back in the available ring (driver area)
        for (uint16_t i = 0; i < buffers; ++i)
        {
            receiveQueue->driver_area->ring_buffer[(receiveQueue->driver_area->index++) % receiveQueue->size] = descIndex + i;
            // restore descriptor settings
            receiveQueue->descriptor_area[(descIndex + i) % receiveQueue->size].flags = VIRTQ_DESC_F_DEVICE_WRITE_ONLY;
            receiveQueue->descriptor_area[(descIndex + i) % receiveQueue->size].length = 1526;
            receiveQueue->descriptor_area[(descIndex + i) % receiveQueue->size].next = 0;
        }

        receiveQueue->last_device_index++;
    }

    // notify the device that we've updated the availaible ring index
    VNet_Write_Register(REG_QUEUE_NOTIFY, VIRTIO_NET_RECEIVE_QUEUE_INDEX);
}

void VirtIO_Net_GetMAC(uint32_t *ptr)
{
    memcpy(ptr, virtio_nic.mac_addr, 6);
}