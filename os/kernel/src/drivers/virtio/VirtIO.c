#include "Virtio.h"

// TODO: Error-checking
void VirtIO_Allocate_Virtqueue(virtq *virtqueue, uint16_t queueSize)
{
    // Zero virtqueue structure
    memset(virtqueue, 0, sizeof(virtq));

    // determine size of virtqueue in bytes (see 2.4 Virtqueues in virtio spec)

    // virtqueues consist of:
    // Descriptor table
    // Available Ring
    // Used Ring
    //  - the above structures have alignment requirements we need to ensure we're fulfilling.

    // descriptor table must be aligned on a 16-byte boundary. Since the virtqueue itself must be aligned on a 4096-byte boundary,
    // this alignment will be guaranteed.
    uint32_t descriptorTableSize = 16 * queueSize;

    // driver area (AKA available ring) must be aligned on a 2-byte boundary, which it always will be because descriptorSize will be aligned to 
    // a 4096-byte boundary and its size will be a multiple of 16.
    uint32_t driverAreaSize = 2 * queueSize + 6;

    // device area (AKA used ring) must be aligned on a 4096-byte boundary (because this is a legacy driver), which it probably won't be
    uint32_t driverAreaPadding = 0;
    if ((driverAreaSize + descriptorTableSize) % 4096 != 0)
        driverAreaPadding = 4096 - ((driverAreaSize + descriptorTableSize) % 4096);

    uint32_t deviceAreaSize = 8 * queueSize + 6;

    uint32_t virtqueueByteSize = descriptorTableSize + driverAreaSize + driverAreaPadding + deviceAreaSize;

    // Allocate memory for virtqueue + extra bytes for 4096-byte alignment
    uint32_t *virtqueue_mem = heap_kernel_alloc(virtqueueByteSize + 4095, 0);

    // Zero virtqueue memory
    memset(virtqueue_mem, 0, virtqueueByteSize + 4095);

    // Get a 4096-byte aligned block of memory
    //virtq *virtqueue = virtqueue_mem;
    if ((uint32_t)virtqueue_mem % 4096)
    {
        virtqueue_mem = (uint8_t*)((uint32_t)virtqueue_mem + 4096 - (uint32_t)virtqueue_mem % 4096);
    }

    // setup elements of virtqueue
    virtqueue->elements = queueSize;
    // descriptors will point to the first byte of virtqueue_mem
    virtqueue->descriptors = (virtq_descriptor *)virtqueue_mem;
    // driverArea (AKA Available Ring) will follow descriptors
    virtqueue->driverArea = (virtq_driver_area *)((uint32_t)virtqueue_mem + descriptorTableSize);
    // deviceArea will follow driver area + padding bytes
    virtqueue->deviceArea = (virtq_device_area *)((uint32_t)virtqueue->driverArea + driverAreaSize + driverAreaPadding);

    virtqueue->byteSize = virtqueueByteSize;
}
