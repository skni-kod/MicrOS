#ifndef VIRTIO_GENERIC
#define VIRTIO_GENERIC
#include <stdint.h>
#include "../pci/pci.h"
#include "../assembly/io.h"
#include "../logger/logger.h"
#include "../debug_helpers/library/kernel_stdio.h"
#include "../cpu/dma/dma.h"

// Registers (generic virtio) - note: this is considered the "legacy interface" and is defined in 4.1.4.8 of the virtio spec
#define REG_DEVICE_FEATURES 0x00
#define REG_GUEST_FEATURES  0x04
#define REG_QUEUE_ADDRESS   0x08
#define REG_QUEUE_SIZE      0x0C
#define REG_QUEUE_SELECT    0x0E
#define REG_QUEUE_NOTIFY    0x10
#define REG_DEVICE_STATUS   0x12
#define REG_ISR_STATUS      0x13

// REG_DEVICE_STATUS flags
#define STATUS_RESET_DEVICE         0       /* Can't be combined with other flags; reset is accomplished by writing 0 to the status register. */
#define	STATUS_DEVICE_ACKNOWLEDGED  0x01
#define	STATUS_DRIVER_LOADED        0x02
#define	STATUS_DRIVER_READY         0x04
#define STATUS_FEATURES_OK          0x08
#define	STATUS_DEVICE_ERROR         0x40
#define	STATUS_DRIVER_FAILED        0x80

// ISR bits
#define VIRTIO_ISR_VIRTQ_USED               1
#define VIRTIO_ISR_CONFIG_CHANGED           2

// Flags for virtqueue descriptors
/* This marks a buffer as continuing via the next field. */
#define VIRTQ_DESC_F_NEXT                   1
/* This marks a buffer as device write-only (otherwise device read-only). */
#define VIRTQ_DESC_F_DEVICE_WRITE_ONLY      2
#define VIRTQ_DESC_F_DEVICE_READ_ONLY       0
/* This means the buffer contains a list of buffer descriptors. */
#define VIRTQ_DESC_F_INDIRECT               4


// Reserved feature bits
#define VIRTIO_F_RING_INDIRECT_DESC (1 << 28) /* Negotiating this feature indicates that the driver can use descriptors 
// with the VIRTQ_DESC_F_INDIRECT flag set, as described in 2.6.5.3 Indirect Descriptors and 2.7.7 Indirect Flag : Scatter - Gather Support. */
#define VIRTIO_F_RING_EVENT_IDX     (1 << 29) /* This feature enables the used_event and the avail_event fields as described in 2.6.7, 2.6.8 and 2.7.10. */
#define VIRTIO_F_VERSION_1          0x100000000

typedef struct virtio_pci_cap
{
    uint8_t cap_vndr;   /* Generic PCI field: PCI_CAP_ID_VNDR */
    uint8_t cap_next;   /* Generic PCI field: next ptr. */
    uint8_t cap_len;    /* Generic PCI field: capability length */
    uint8_t cfg_type;   /* Identifies the structure. */
    uint8_t bar;        /* Where to find it. */
    uint8_t padding[3]; /* Pad to full dword. */
    uint32_t offset;    /* Offset within bar. */
    uint32_t length;    /* Length of the structure, in bytes. */
} virtio_pci_cap;

// defines for cfg_type of virtio_pci_cap
/* Common configuration */
#define VIRTIO_PCI_CAP_COMMON_CFG   1
/* Notifications */
#define VIRTIO_PCI_CAP_NOTIFY_CFG   2
/* ISR Status */
#define VIRTIO_PCI_CAP_ISR_CFG      3
/* Device specific configuration */
#define VIRTIO_PCI_CAP_DEVICE_CFG   4
/* PCI configuration access */
#define VIRTIO_PCI_CAP_PCI_CFG      5


typedef struct virtio_pci_common_cfg 
{
    /* About the whole device. */
    uint32_t device_feature_select; /* read-write */
    uint32_t device_feature;        /* read-only for driver */
    uint32_t driver_feature_select; /* read-write */
    uint32_t driver_feature;        /* read-write */
    uint16_t msix_config;           /* read-write */
    uint16_t num_queues;            /* read-only for driver */
    uint8_t device_status;          /* read-write */
    uint8_t config_generation;      /* read-only for driver */
                          
    /* About a specific virtqueue. */
    uint16_t queue_select;      /* read-write */
    uint16_t queue_size;        /* read-write */
    uint16_t queue_msix_vector; /* read-write */
    uint16_t queue_enable;      /* read-write */
    uint16_t queue_notify_off;  /* read-only for driver */
    uint64_t queue_desc;        /* read-write */
    uint64_t queue_driver;      /* read-write */
    uint64_t queue_device;      /* read-write */
} virtio_pci_common_cfg;

typedef struct virtio_pci_notify_cap 
{
    struct virtio_pci_cap cap;
    uint32_t notify_off_multiplier; /* Multiplier for queue_notify_off. */
} virtio_pci_notify_cap;

typedef struct virtq_descriptor
{
    /* Address (guest-physical). */
    uint64_t address;
    uint32_t length;

    /* The flags as indicated above. */
    uint16_t flags;
    /* Next field if flags & NEXT */
    uint16_t next;
} virtq_descriptor;


// Flag for virtq_driver_area:
#define VIRTQ_AVAIL_F_NO_INTERRUPT          1

// The driver area AKA Available Ring is only written by the driver and read by the device
typedef struct virtq_driver_area
{
    uint16_t flags;
    // index indicates where the driver will put the next entry in the ring (wrapping to the beginning of the ring as needed). It's value only ever increases.
    uint16_t index;
    // ringBuffer is a ring-buffer that is queue-size elements long. Each entry refers to the head of a descriptor chain.
    uint16_t ringBuffer[1];
    // ringBuffer will be allocated in-place and take up (2 * (queue size)) bytes

    // uint16_t used_event would be present after ringBuffer, but only if VIRTIO_F_EVENT_IDX is negotiated. (we don't use it here)
} virtq_driver_area, virtq_available_ring;


typedef struct virtq_device_element
{
    // index is the index into the descriptor table of used descriptor chain
    uint32_t index;
    // length is the total length of bytes written to the descriptor chain
    uint32_t length;
} virtq_device_element, virtq_used_elem;

// Flag for virtq_device_area
#define VIRTQ_USED_F_NO_NOTIFY              1

// The device area AKA Used Ring is only written by the device and read by the driver
typedef struct virtq_device_area
{
    uint16_t flags;
    uint16_t index;
    virtq_device_element ringBuffer[1];
    // ringBuffer will be allocated in-place and take up (4 * (queue size)) bytes. Each element describes a used descriptor, including the index and length.

    // uint16_t avail_event will be present after ringBuffer. It's only used if VIRTIO_F_EVENT_IDX is negotiated. (we don't use it here)
} virtq_device_area, virtq_used_ring;

typedef struct virtq
{
    uint16_t elements;              // number of queue elements AKA queue size
    virtq_descriptor *descriptors;  // will be an elements-long array of descriptors
    virtq_driver_area *driverArea;  // extra data supplied by the driver to the device. AKA Available Ring
    virtq_device_area *deviceArea;  // extra data supplied by the device to the driver. AKA Used Ring
    uint16_t nextDescriptor;        // index of the next slot in the descriptor table we should use
    uint16_t lastDeviceAreaIndex;   // the last value of deviceArea->index we've seen. If this value doesn't equal deviceArea->index, 
                                    // the device has added used descriptors to deviceArea->ringBuffer.
    uint32_t byteSize; // for debug
} virtq;


void VirtIO_Allocate_Virtqueue(virtq *virtqueue, uint16_t queueSize);
#endif