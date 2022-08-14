/*
    @JakubPrzystasz
    Created: 28.07.2021
    Last modified: 13.02.2022
*/
#include "virtio-pci-nic.h"

pci_device pci_virtio_nic_device;
virtio_nic_dev virtio_nic;
net_device_t *virtio_nic_net_device;

virtq *receive_queue;
virtq *transmit_queue;

bool virtio_nic_init(net_device_t *net_dev)
{

    // Prevent for re-init
    if (pci_virtio_nic_device.vendor_id != 0)
        return false;

    for (uint8_t i = 0; i < pci_get_number_of_devices(); i++)
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

    // Device not present in PCI bus
    if (pci_virtio_nic_device.vendor_id == 0)
        return false;

    // Get device configuration address
    virtio_nic.bar_type = pci_virtio_nic_device.base_addres_0 & (0x1);

    if (!virtio_nic.bar_type)
        // Driver can only configure device via IO ports.
        return false;

    virtio_nic.io_base = pci_virtio_nic_device.base_addres_0 & (~0x3);

    // Reset the virtio-network device
    virtio_nic_reg_write(REG_DEVICE_STATUS, STATUS_RESET_DEVICE);

    // Set the acknowledge status bit
    virtio_nic_reg_write(REG_DEVICE_STATUS, STATUS_DEVICE_ACKNOWLEDGE);

    // Set the driver status bit
    virtio_nic_reg_write(REG_DEVICE_STATUS, STATUS_DEVICE_ACKNOWLEDGE | STATUS_DRIVER_LOADED);

    // Read the feature bits
    uint32_t features = virtio_nic_reg_read(REG_DEVICE_FEATURES);

    // Make sure the features we need are supported
    if ((features & REQUIRED_FEATURES) != REQUIRED_FEATURES)
    {
        virtio_nic_reg_write(REG_DEVICE_STATUS, STATUS_DRIVER_FAILED);
        return false;
    }

    // Tell the device what features we'll be using
    virtio_nic_reg_write(REG_DRIVER_FEATURES, REQUIRED_FEATURES);

    // PCI bus mastering
    // uint32_t pci_bus_config = pci_io_in(&pci_virtio_nic_device, 1);
    // pci_bus_config |= 0x4;
    // pci_io_out(&pci_virtio_nic_device, 1, pci_bus_config);
    pci_busmaster_set(&pci_virtio_nic_device, true);

    /*
        // Init virtqueues (see 4.1.5.1.3 of virtio-v1.0-cs04.pdf)
        // No VIRTIO_NET_F_MQ, using 3 virtqueues: receive, transmit, and control
    */
    receive_queue = heap_kernel_alloc(sizeof(virtq), 0);
    transmit_queue = heap_kernel_alloc(sizeof(virtq), 0);

    virtio_nic_init_queue(receive_queue, 0);
    virtio_nic_init_queue(transmit_queue, 1);
    virtio_nic_setup_buffers(32);
    receive_queue->device_area->flags = 0;

    // Get mac address
    for (uint8_t i = 0; i < 6; i++)
        virtio_nic.mac_addr[i] = io_in_byte(virtio_nic.io_base + 0x14 + i);

    net_dev->device_name = heap_kernel_alloc((uint32_t)strlen(VIRTIO_NET_DEVICE_NAME) + 1, 0);
    strcpy(net_dev->device_name, VIRTIO_NET_DEVICE_NAME);
    memcpy(net_dev->configuration->mac_address, virtio_nic.mac_addr, MAC_ADDRESS_SIZE);
    net_dev->dpi.send = &virtio_nic_send;
    virtio_nic_net_device = net_dev;

    // Driver's ready to work
    virtio_nic_reg_write(REG_DEVICE_STATUS, STATUS_DRIVER_OK);
    virtio_nic_reg_write(REG_QUEUE_NOTIFY, VIRTIO_NET_RECEIVE_QUEUE_INDEX);

    // Enable IRQ
    uint32_t irq_num = pci_virtio_nic_device.interrupt_line;
    pic_enable_irq(irq_num);
    idt_attach_interrupt_handler(irq_num, virtio_nic_irq_handler);

    return true;
}

bool virtio_nic_irq_handler()
{
    if (virtio_nic_reg_read(REG_ISR_STATUS) & VIRTIO_ISR_VIRTQ_USED)
    {
        // Transmit packet
        while (transmit_queue->device_area->index != transmit_queue->last_device_index)
        {
            // Free buffers after transmitting
            uint16_t ring_index, descriptor_index;
            ring_index = transmit_queue->last_device_index % transmit_queue->size;
            descriptor_index = (uint16_t)transmit_queue->device_area->ring_buffer[ring_index].index;
            descriptor_index %= transmit_queue->size;

            transmit_queue->descriptor_area[descriptor_index].address = 0xFFFFFFFFFFFFFFFF;
            transmit_queue->descriptor_area[descriptor_index].length = 0;

            while (transmit_queue->descriptor_area[descriptor_index].next)
            {
                transmit_queue->descriptor_area[descriptor_index++].next = 0;
                descriptor_index %= transmit_queue->size;
                transmit_queue->descriptor_area[descriptor_index].address = 0xFFFFFFFFFFFFFFFF;
                transmit_queue->descriptor_area[descriptor_index].length = 0;
            }
            transmit_queue->device_area++;
        }

        // Receive packet
        if (receive_queue->device_area->index != receive_queue->last_device_index && virtio_nic_net_device->configuration->mode & 0x1)
            virtio_nic_receive();
    }

    return true;
}

void virtio_nic_init_queue(virtq *virtqueue, uint16_t queueIndex)
{
    int16_t queueSize = -1;

    // access the current queue
    virtio_nic_reg_write(REG_QUEUE_SELECT, queueIndex);

    // get the size of the current queue
    queueSize = (uint16_t)virtio_nic_reg_read(REG_QUEUE_SIZE);

    if (!queueSize)
        return;

    // Allocate and initialize the queue
    virtio_setup_queue(virtqueue, queueSize);

    uint32_t addr = (uint32_t)((uint32_t)virtqueue->descriptor_area - DMA_ADDRESS_OFFSET) / 4096;

    virtio_nic_reg_write(REG_QUEUE_ADDRESS, addr);
}

void virtio_nic_setup_buffers(uint16_t buffers_count)
{
    // Allocate and add buffers to receive queue
    for (uint16_t i = 0; i < buffers_count; ++i)
    {
        uint32_t buffer = ((uint32_t)heap_kernel_alloc(VIRTIO_NET_MTU, 0) - DMA_ADDRESS_OFFSET);

        // Add buffer to the descriptor table
        receive_queue->descriptor_area[i].address = (uint64_t)buffer;
        receive_queue->descriptor_area[i].flags = VIRTQ_DESC_F_DEVICE_WRITE_ONLY;
        receive_queue->descriptor_area[i].length = VIRTIO_NET_MTU;
        receive_queue->descriptor_area[i].next = 0;

        // Add index of descriptor to the driver ring
        receive_queue->driver_area->ring_buffer[i] = i;
    }

    // Update next available index
    receive_queue->driver_area->index = buffers_count;
    virtio_nic_reg_write(REG_QUEUE_NOTIFY, VIRTIO_NET_RECEIVE_QUEUE_INDEX);
}

void virtio_nic_send(nic_data_t *data)
{
    // Allocate a buffer for the header
    virtio_nic_net_header *net_buffer = heap_kernel_alloc(VIRTIO_NET_HEADER_SIZE, 0);

    // Set parameters of net_buffer
    memset(net_buffer, 0, sizeof(virtio_nic_net_header));

    // Get indices for the next two descriptor_area
    uint16_t descriptor_index = transmit_queue->next_descriptor % transmit_queue->size;
    uint16_t descriptor_index2 = (descriptor_index + 1) % transmit_queue->size;
    transmit_queue->next_descriptor += 2;

    // Get index for the next entry into the device ring
    uint16_t index = transmit_queue->driver_area->index % transmit_queue->size;

    // fill descriptor with net header
    transmit_queue->descriptor_area[descriptor_index].address = (uint64_t)((uint32_t)net_buffer - (uint32_t)DMA_ADDRESS_OFFSET);
    if (transmit_queue->descriptor_area[descriptor_index].address == 0xdeadbeef || transmit_queue->descriptor_area[descriptor_index].address == 0xdeadbeefdeadbeef)
        return;
    transmit_queue->descriptor_area[descriptor_index].flags = VIRTQ_DESC_F_NEXT;
    transmit_queue->descriptor_area[descriptor_index].length = VIRTIO_NET_HEADER_SIZE;
    transmit_queue->descriptor_area[descriptor_index].next = descriptor_index2;

    // fill descriptor with ethernet packet
    transmit_queue->descriptor_area[descriptor_index2].address = (uint64_t)(GET_PHYSICAL_ADDRESS(data->frame));
    if (transmit_queue->descriptor_area[descriptor_index2].address == 0xdeadbeef || transmit_queue->descriptor_area[descriptor_index].address == 0xdeadbeefdeadbeef)
        return;
    transmit_queue->descriptor_area[descriptor_index2].flags = 0;
    transmit_queue->descriptor_area[descriptor_index2].length = data->length;
    transmit_queue->descriptor_area[descriptor_index2].next = 0;

    // Add descriptor chain to the available ring
    transmit_queue->driver_area->ring_buffer[index] = descriptor_index;

    // Increase available ring index and notify the device
    transmit_queue->driver_area->index++;

    virtio_nic_reg_write(REG_QUEUE_NOTIFY, VIRTIO_NET_TRANSMIT_QUEUE_INDEX);
}

void virtio_nic_receive()
{
    while (receive_queue->device_area->index != receive_queue->last_device_index)
    {
        uint16_t ring_buffer_index = receive_queue->last_device_index % receive_queue->size;
        uint16_t descriptor_index = (uint16_t)receive_queue->device_area->ring_buffer[ring_buffer_index].index;
        uint32_t *buffer_address = (uint32_t *)((uint32_t)receive_queue->descriptor_area[descriptor_index % receive_queue->size].address + DMA_ADDRESS_OFFSET);
        uint16_t buffers = ((virtio_nic_net_header *)buffer_address)->buffers_count;

        if (buffers > 1)
        {
            uint32_t buffer_size = 0;

            // get length of entire buffer
            for (int i = 0; i < buffers; ++i)
            {
                buffer_size += receive_queue->descriptor_area[(descriptor_index + i) % receive_queue->size].length;
            }

            buffer_address = heap_kernel_alloc(buffer_size, 0);
            if (!buffer_address)
            {
                logger_log_warning("Not enough memory\n");
                return;
            }

            // copy buffer pieces to one block
            uint32_t offset = 0;
            for (int i = 0; i < buffers; ++i)
            {
                memcpy((void *)((uint32_t)buffer_address + offset),
                       (void *)(uint32_t)receive_queue->descriptor_area[(descriptor_index + i) % receive_queue->size].address,
                       receive_queue->descriptor_area[(descriptor_index + i) % receive_queue->size].length);
                offset += receive_queue->descriptor_area[(descriptor_index + i) % receive_queue->size].length;
            }
        }

        // Skip over virtio_nic_net_header to get a pointer to the frame
        uint32_t *data_ptr = (uint32_t *)((uint32_t)buffer_address + (uint32_t)sizeof(virtio_nic_net_header));

        // Get frame length by skipping packet header
        uint32_t size = receive_queue->descriptor_area[descriptor_index % receive_queue->size].length;

        // Copy received data to buffer, data is right after the header with length
        nic_data_t *out =  virtio_nic_net_device->dpi.get_receive_buffer(virtio_nic_net_device, size);
        memcpy((void *)out->frame, data_ptr, size);

        // Place the used descriptor indices back in driver area
        for (uint16_t i = 0; i < buffers; ++i)
        {
            receive_queue->driver_area->ring_buffer[(receive_queue->driver_area->index++) % receive_queue->size] = descriptor_index + i;
            // restore descriptor settings
            receive_queue->descriptor_area[(descriptor_index + i) % receive_queue->size].flags = VIRTQ_DESC_F_DEVICE_WRITE_ONLY;
            receive_queue->descriptor_area[(descriptor_index + i) % receive_queue->size].length = VIRTIO_NET_MTU;
            receive_queue->descriptor_area[(descriptor_index + i) % receive_queue->size].next = 0;
        }

        receive_queue->last_device_index++;

        // Notify network manager about incoming data
        (*virtio_nic_net_device->dpi.receive)(out);
    }

    // notify about updates in device area
    virtio_nic_reg_write(REG_QUEUE_NOTIFY, VIRTIO_NET_RECEIVE_QUEUE_INDEX);
}

void virtio_nic_get_mac(uint8_t *ptr)
{
    memcpy(ptr, virtio_nic.mac_addr, 6);
}

uint32_t virtio_nic_reg_read(uint16_t reg)
{
    if (reg < REG_QUEUE_SIZE)
        return io_in_long(virtio_nic.io_base + reg);
    else if (reg <= REG_QUEUE_NOTIFY)
        return io_in_word(virtio_nic.io_base + reg);
    else
        return io_in_byte(virtio_nic.io_base + reg);
}

void virtio_nic_reg_write(uint16_t reg, uint32_t data)
{
    if (reg < REG_QUEUE_SIZE)
        io_out_long(virtio_nic.io_base + reg, data);
    else if (reg <= REG_QUEUE_NOTIFY)
        io_out_word(virtio_nic.io_base + reg, (uint16_t)data);
    else
        io_out_byte(virtio_nic.io_base + reg, (uint8_t)data);
}
