/*
    @JakubPrzystasz
    Created: 28.07.2021
    Last modified: 13.02.2022
*/
#include "virtio-pci-nic.h"

// pci_device pci_virtio_nic_device;
// virtio_nic_dev virtio_nic;
// net_device_t *virtio_nic_net_device;

// virtq *receive_queue;
// virtq *transmit_queue;

static uint8_t devices_count = 0;
static void **devices;

static char virtio_name[] = VIRTIO_NET_DEVICE_NAME;

bool virtio_nic_probe(net_device_t *(*get_net_device)())
{
    for (uint8_t i = 0; i < pci_get_number_of_devices(); i++)
    {
        pci_device *pci_dev = pci_get_device(i);

        if (pci_dev->config.vendor_id == VIRTIO_NET_DEVICE_VENDOR_ID &&
            pci_dev->config.device_id >= VIRTIO_NET_DEVICE_ID_BEGIN &&
            pci_dev->config.device_id <= VIRTIO_NET_DEVICE_ID_END &&
            pci_dev->config.subsystem_id == VIRTIO_NET_DEVICE_SUBSYSTEM_ID)
        {

            virtio_nic_dev_t *dev = heap_kernel_alloc(sizeof(virtio_nic_dev_t), 0);
            devices = heap_kernel_realloc(devices, sizeof(virtio_nic_dev_t *) * ++devices_count, 0);
            devices[devices_count - 1] = dev;
            memset(dev, 0x0, sizeof(virtio_nic_dev_t));

            dev->pci = pci_dev;
            dev->net = get_net_device();
            dev->net->priv = dev;

            // Enable busmaster
            pci_busmaster_set(dev->pci, true);
            // Enable interrupt
            dev->irq_vector = dev->pci->config.interrupt_line;
            idt_attach_interrupt_handler(dev->irq_vector, virtio_nic_irq_handler);
            pic_enable_irq(dev->irq_vector);

            dev->io_type = (io_type_t)(dev->pci->config.base_addres_0 & (0x1));
            switch (dev->io_type)
            {
            case IO:
                dev->io_base = dev->pci->config.base_addres_0 & (~0x3);
                break;
            case MMAP:
                dev->mem_base = dev->pci->config.base_addres_0 & (~0xf);
                break;
            }

            // Setup device, get mac address
            for (uint8_t i = 0; i < 6; i++)
                dev->net->interface.mac.addr[i] = virtio_nic_reg_read(dev, 0x14 + i);

            memcpy(&dev->net->interface.name, virtio_name, strlen(virtio_name));
            dev->net->dpi.send = &virtio_nic_send;

            // Reset the virtio-network device
            virtio_nic_reg_write(dev, REG_DEVICE_STATUS, STATUS_RESET_DEVICE);

            // Set the acknowledge status bit
            virtio_nic_reg_write(dev, REG_DEVICE_STATUS, STATUS_DEVICE_ACKNOWLEDGE);

            // Set the driver status bit
            virtio_nic_reg_write(dev, REG_DEVICE_STATUS, STATUS_DEVICE_ACKNOWLEDGE | STATUS_DRIVER_LOADED);

            // Read the feature bits
            uint32_t features = virtio_nic_reg_read(dev, REG_DEVICE_FEATURES);

            // Make sure the features we need are supported
            if ((features & VIRTIO_NET_REQUIRED_FEATURES) != VIRTIO_NET_REQUIRED_FEATURES)
            {
                virtio_nic_reg_write(dev, REG_DEVICE_STATUS, STATUS_DRIVER_FAILED);
                return false;
            }

            // Tell the device what features we'll be using
            virtio_nic_reg_write(dev, REG_DRIVER_FEATURES, VIRTIO_NET_REQUIRED_FEATURES);

            /*
                // Init virtqueues (see 4.1.5.1.3 of virtio-v1.0-cs04.pdf)
                // No VIRTIO_NET_F_MQ, using 3 virtqueues: receive, transmit, and control
            */
            dev->rx = virtio_nic_init_queue(dev, 0);
            if (!dev->rx)
                return false;

            dev->tx = virtio_nic_init_queue(dev, 1);
            if (!dev->tx)
                return false;

            virtio_nic_setup_buffers(dev, 32);
            dev->rx->device_area->flags = 0;

            // Driver's ready to work
            virtio_nic_reg_write(dev, REG_DEVICE_STATUS, STATUS_DRIVER_OK);
            virtio_nic_reg_write(dev, REG_QUEUE_NOTIFY, VIRTIO_NET_RECEIVE_QUEUE_INDEX);
        }
    }

    if (devices_count)
        return true;
    return false;
}

bool virtio_nic_irq_handler(interrupt_state *state)
{
    for (uint32_t dev_id = 0; dev_id < devices_count; dev_id++)
    {
        virtio_nic_dev_t *dev = devices[dev_id];
        if (virtio_nic_reg_read(dev, REG_ISR_STATUS) & VIRTIO_ISR_VIRTQ_USED)
        {
            // Transmitt packet
            while (dev->rx->device_area->index != dev->tx->last_device_index)
            {
                // Free buffers after transmitting
                uint16_t ring_index, descriptor_index;
                ring_index = dev->tx->last_device_index % dev->tx->size;
                descriptor_index = (uint16_t)dev->tx->device_area->ring_buffer[ring_index].index;
                descriptor_index %= dev->tx->size;

                dev->tx->descriptor_area[descriptor_index].address = 0xFFFFFFFFFFFFFFFF;
                dev->tx->descriptor_area[descriptor_index].length = 0;

                while (dev->tx->descriptor_area[descriptor_index].next)
                {
                    dev->tx->descriptor_area[descriptor_index++].next = 0;
                    descriptor_index %= dev->tx->size;
                    dev->tx->descriptor_area[descriptor_index].address = 0xFFFFFFFFFFFFFFFF;
                    dev->tx->descriptor_area[descriptor_index].length = 0;
                }
                dev->tx->device_area++;
            }

            // Receive packet
            if (dev->rx->device_area->index != dev->rx->last_device_index &&
                dev->net->interface.mode.receive)
                virtio_nic_receive(dev);

            return true;
        }
    }
    return false;
}

virtq *virtio_nic_init_queue(virtio_nic_dev_t *dev, uint16_t queueIndex)
{
    int16_t queueSize = 0;
    // get size of queue
    virtio_nic_reg_write(dev, REG_QUEUE_SELECT, queueIndex);
    queueSize = (uint16_t)virtio_nic_reg_read(dev, REG_QUEUE_SIZE);

    if (!queueSize)
        return;

    // allocate queue
    virtq *virtqueue = 0;
    virtqueue = heap_kernel_alloc(sizeof(virtq), 0);
    if (!virtqueue)
        return;

    virtio_setup_queue(virtqueue, queueSize);

    uint32_t addr = (uint32_t)((uint32_t)virtqueue->descriptor_area - DMA_ADDRESS_OFFSET) / 4096;
    virtio_nic_reg_write(dev, REG_QUEUE_ADDRESS, addr);

    return virtqueue;
}

void virtio_nic_setup_buffers(virtio_nic_dev_t *dev, uint16_t buffers_count)
{
    // Allocate and add buffers to receive queue
    for (uint16_t i = 0; i < buffers_count; ++i)
    {
        uint32_t buffer = ((uint32_t)heap_kernel_alloc(VIRTIO_NET_MTU, 0) - DMA_ADDRESS_OFFSET);

        // Add buffer to the descriptor table
        dev->rx->descriptor_area[i].address = (uint64_t)buffer;
        dev->rx->descriptor_area[i].flags = VIRTQ_DESC_F_DEVICE_WRITE_ONLY;
        dev->rx->descriptor_area[i].length = VIRTIO_NET_MTU;
        dev->rx->descriptor_area[i].next = 0;

        // Add index of descriptor to the driver ring
        dev->rx->driver_area->ring_buffer[i] = i;
    }

    // Update next available index
    dev->rx->driver_area->index = buffers_count;
    virtio_nic_reg_write(dev, REG_QUEUE_NOTIFY, VIRTIO_NET_RECEIVE_QUEUE_INDEX);
}

void virtio_nic_send(nic_data_t *data)
{
    // Allocate a buffer for the header
    virtio_nic_net_header_t *net_buffer = heap_kernel_alloc(VIRTIO_NET_HEADER_SIZE, 0);
    virtio_nic_dev_t *dev = data->device->priv;
    // Set parameters of net_buffer
    memset(net_buffer, 0, sizeof(virtio_nic_net_header_t));

    // Get indices for the next two descriptor_area
    uint16_t descriptor_index = dev->tx->next_descriptor % dev->tx->size;
    uint16_t descriptor_index2 = (descriptor_index + 1) % dev->tx->size;
    dev->tx->next_descriptor += 2;

    // Get index for the next entry into the device ring
    uint16_t index = dev->tx->driver_area->index % dev->tx->size;

    // fill descriptor with net header
    dev->tx->descriptor_area[descriptor_index].address = (uint64_t)((uint32_t)net_buffer - (uint32_t)DMA_ADDRESS_OFFSET);
    if (dev->tx->descriptor_area[descriptor_index].address == 0xdeadbeef || dev->tx->descriptor_area[descriptor_index].address == 0xdeadbeefdeadbeef)
        return;
    dev->tx->descriptor_area[descriptor_index].flags = VIRTQ_DESC_F_NEXT;
    dev->tx->descriptor_area[descriptor_index].length = VIRTIO_NET_HEADER_SIZE;
    dev->tx->descriptor_area[descriptor_index].next = descriptor_index2;

    // fill descriptor with ethernet packet
    dev->tx->descriptor_area[descriptor_index2].address = (uint64_t)(GET_PHYSICAL_ADDRESS(data->frame));
    if (dev->tx->descriptor_area[descriptor_index2].address == 0xdeadbeef || dev->tx->descriptor_area[descriptor_index].address == 0xdeadbeefdeadbeef)
        return;
    dev->tx->descriptor_area[descriptor_index2].flags = 0;
    dev->tx->descriptor_area[descriptor_index2].length = data->length;
    dev->tx->descriptor_area[descriptor_index2].next = 0;

    // Add descriptor chain to the available ring
    dev->tx->driver_area->ring_buffer[index] = descriptor_index;

    // Increase available ring index and notify the device
    dev->tx->driver_area->index++;

    virtio_nic_reg_write(dev, REG_QUEUE_NOTIFY, VIRTIO_NET_TRANSMIT_QUEUE_INDEX);
}

void virtio_nic_receive(virtio_nic_dev_t *dev)
{
    while (dev->rx->device_area->index != dev->rx->last_device_index)
    {
        uint16_t ring_buffer_index = dev->rx->last_device_index % dev->rx->size;
        uint16_t descriptor_index = (uint16_t)dev->rx->device_area->ring_buffer[ring_buffer_index].index;
        uint32_t *buffer_address = (uint32_t *)((uint32_t)dev->rx->descriptor_area[descriptor_index % dev->rx->size].address + DMA_ADDRESS_OFFSET);
        uint16_t buffers = ((virtio_nic_net_header_t *)buffer_address)->num_buffers;

        if (buffers > 1)
        {
            uint32_t buffer_size = 0;

            // get length of entire buffer
            for (int i = 0; i < buffers; ++i)
                buffer_size += dev->rx->descriptor_area[(descriptor_index + i) % dev->rx->size].length;

            buffer_address = heap_kernel_alloc(buffer_size, 0);
            if (!buffer_address)
                return;

            // copy buffer pieces to one block
            uint32_t offset = 0;
            for (int i = 0; i < buffers; ++i)
            {
                memcpy((void *)((uint32_t)buffer_address + offset),
                       (void *)(uint32_t)dev->rx->descriptor_area[(descriptor_index + i) % dev->rx->size].address,
                       dev->rx->descriptor_area[(descriptor_index + i) % dev->rx->size].length);
                offset += dev->rx->descriptor_area[(descriptor_index + i) % dev->rx->size].length;
            }
        }

        // Skip over virtio_nic_net_header to get a pointer to the frame
        uint32_t *data_ptr = (uint32_t *)((uint32_t)buffer_address + (uint32_t)sizeof(virtio_nic_net_header_t));

        // Get frame length by skipping packet header
        uint32_t size = dev->rx->descriptor_area[descriptor_index % dev->rx->size].length;

        // Copy received data to buffer, data is right after the header with length
        nic_data_t *out = dev->net->dpi.get_receive_buffer(dev->net);

        memcpy((void *)out->frame, data_ptr, size);

        // Place the used descriptor indices back in driver area
        for (uint16_t i = 0; i < buffers; ++i)
        {
            dev->rx->driver_area->ring_buffer[(dev->rx->driver_area->index++) % dev->rx->size] = descriptor_index + i;
            // restore descriptor settings
            dev->rx->descriptor_area[(descriptor_index + i) % dev->rx->size].flags = VIRTQ_DESC_F_DEVICE_WRITE_ONLY;
            dev->rx->descriptor_area[(descriptor_index + i) % dev->rx->size].length = VIRTIO_NET_MTU;
            dev->rx->descriptor_area[(descriptor_index + i) % dev->rx->size].next = 0;
        }

        dev->rx->last_device_index++;

        // Notify network manager about incoming data
        (*dev->net->dpi.receive)(out);
    }

    // notify about updates in device area
    virtio_nic_reg_write(dev, REG_QUEUE_NOTIFY, VIRTIO_NET_RECEIVE_QUEUE_INDEX);
}

uint32_t virtio_nic_reg_read(virtio_nic_dev_t *dev, uint16_t reg)
{
    switch (dev->io_type)
    {
    case IO:
        if (reg < REG_QUEUE_SIZE)
            return io_in_long(dev->io_base + reg);
        else if (reg <= REG_QUEUE_NOTIFY)
            return io_in_word(dev->io_base + reg);
        else
            return io_in_byte(dev->io_base + reg);
    case MMAP:
        // not implemented
        break;
    };
}

void virtio_nic_reg_write(virtio_nic_dev_t *dev, uint16_t reg, uint32_t data)
{
    switch (dev->io_type)
    {
    case IO:
        if (reg < REG_QUEUE_SIZE)
            io_out_long(dev->io_base + reg, data);
        else if (reg <= REG_QUEUE_NOTIFY)
            io_out_word(dev->io_base + reg, (uint16_t)data);
        else
            io_out_byte(dev->io_base + reg, (uint8_t)data);
        break;
    case MMAP:
        // not implemented
        break;
    };
}
