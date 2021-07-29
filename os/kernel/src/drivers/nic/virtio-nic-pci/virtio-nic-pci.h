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
#include "../../../debug_helpers/library/kernel_stdio.h"
#include <stdlib.h>

#define VIRTIO_NIC_DEVICE_VENDOR_ID 0x1AF4
#define VIRTIO_NIC_DEVICE_CLASS_ID 0x2
#define VIRTIO_NIC_DEVICE_ID_BEGIN 0x1000
#define VIRTIO_NIC_DEVICE_ID_END 0x103F
#define VIRTIO_NIC_DEVICE_NAME "VirtIO NIC"

// Network-device-specific registers:
#define	REG_MAC_1           0x14
#define	REG_MAC_2           0x15
#define	REG_MAC_3           0x16
#define	REG_MAC_4           0x17
#define	REG_MAC_5           0x18
#define	REG_MAC_6           0x19
#define REG_NIC_STATUS      0x1A

// Feature bits (See 5.1.3 of virtio-v1.0-cs04.pdf)
#define VIRTIO_NET_F_GUEST_TSO6             0x100
#define VIRTIO_NET_F_GUEST_ECN              0x200
#define VIRTIO_NET_F_GUEST_UFO              0x400
#define VIRTIO_NET_F_HOST_TSO4              0x800
#define VIRTIO_NET_F_HOST_TSO6              0x1000
#define VIRTIO_NET_F_HOST_ECN               0x2000
#define VIRTIO_NET_F_HOST_UFO               0x4000
#define VIRTIO_NET_F_MRG_RXBUF              0x8000
#define VIRTIO_NET_F_STATUS                 0x10000
#define VIRTIO_NET_F_CTRL_VQ                0x20000
#define VIRTIO_NET_F_CTRL_RX                0x40000
#define VIRTIO_NET_F_CTRL_VLAN              0x80000
#define VIRTIO_NET_F_GUEST_ANNOUNCE         0x200000
#define VIRTIO_NET_F_MQ                     0x400000
#define VIRTIO_NET_F_CTRL_MAC_ADDR          0x800000
#define VIRTIO_F_ANY_LAYOUT                 (1 << 27)

// These are the features required by this driver (VirtualBox is borken and won't work without VIRTIO_NET_F_MRG_RXBUF) 
#undef REQUIRED_FEATURES
#define REQUIRED_FEATURES (/*VIRTIO_NET_F_CSUM |*/ VIRTIO_NET_F_MAC | VIRTIO_NET_F_MRG_RXBUF)


// Flags for virtio_net_hdr
#define VIRTIO_NET_HDR_F_NEEDS_CSUM 1
#define VIRTIO_NET_HDR_F_DATA_VALID 2
#define VIRTIO_NET_HDR_F_RSC_INFO   4

// Values for virtio_net_hdr gso_type
#define VIRTIO_NET_HDR_GSO_NONE     0
#define VIRTIO_NET_HDR_GSO_TCPV4    1
#define VIRTIO_NET_HDR_GSO_UDP      3
#define VIRTIO_NET_HDR_GSO_TCPV6    4
#define VIRTIO_NET_HDR_GSO_ECN      0x80

// Every packet sent or received must be preceded by a virtio_net_hdr
typedef struct virtio_net_hdr
{
    uint8_t flags;
    uint8_t gso_type;
    uint16_t hdr_len;
    uint16_t gso_size;
    uint16_t csum_start;
    uint16_t csum_offset;
    uint16_t num_buffers;  // num_buffer is not part of struct if VIRTIO_NET_F_MRG_RXBUF isn't negotiated
} virtio_net_hdr;


//Device FEATURES
/* VIRTIO_NET_F_CSUM (0) Device handles packets with partial checksum. This “checksum offload” is a common feature on modern network cards. */
#define VIRTIO_NET_F_CSUM (1 << 0)
/*VIRTIO_NET_F_GUEST_CSUM (1) Driver handles packets with partial checksum.*/
#define VIRTIO_NET_F_GUEST_CSUM (1 << 1)
/* IRTIO_NET_F_CTRL_GUEST_OFFLOADS 0x4(2) Control channel offloads reconfiguration support. */
#define VIRTIO_NET_F_CTRL_GUEST_OFFLOADS (1 << 2)
/* VIRTIO_NET_F_MTU(3) Device maximum MTU reporting is supported. If offered by the device, device advises driver about the value of its maximum MTU. If negotiated, the driver uses mtu as the maximum MTU value.*/
#define VIRTIO_NET_F_MTU (1 << 3)
/*VIRTIO_NET_F_MAC (5) Device has given MAC address. */
#define VIRTIO_NET_F_MAC (1 << 5)
/*VIRTIO_NET_F_GUEST_TSO4 (7) Driver can receive TSOv4.*/
#define VIRTIO_NET_F_GUEST_TSO4 (1 << 7)
/*VIRTIO_NET_F_GUEST_TSO6 (8) Driver can receive TSOv6.
VIRTIO_NET_F_GUEST_ECN (9) Driver can receive TSO with ECN.
VIRTIO_NET_F_GUEST_UFO (10) Driver can receive UFO.
VIRTIO_NET_F_HOST_TSO4 (11) Device can receive TSOv4.
VIRTIO_NET_F_HOST_TSO6 (12) Device can receive TSOv6.
VIRTIO_NET_F_HOST_ECN (13) Device can receive TSO with ECN.
VIRTIO_NET_F_HOST_UFO (14) Device can receive UFO.
VIRTIO_NET_F_MRG_RXBUF (15) Driver can merge receive buffers.
VIRTIO_NET_F_STATUS (16) Configuration status field is available.
VIRTIO_NET_F_CTRL_VQ (17) Control channel is available.
VIRTIO_NET_F_CTRL_RX (18) Control channel RX mode support.
VIRTIO_NET_F_CTRL_VLAN (19) Control channel VLAN filtering.
VIRTIO_NET_F_GUEST_ANNOUNCE(21) Driver can send gratuitous packets.
VIRTIO_NET_F_MQ(22) Device supports multiqueue with automatic receive steering.
VIRTIO_NET_F_CTRL_MAC_ADDR(23) Set MAC address through control channel.
VIRTIO_NET_F_RSC_EXT(61) Device can process duplicated ACKs and report number of coalesced segments
and duplicated ACKs
VIRTIO_NET_F_STANDBY(62) Device may act as a standby for a primary device with the same MAC
address.
*/



typedef struct virtio_nic_dev
{
    uint8_t bar_type;
    uint16_t io_base;
    uint32_t mem_base;
    uint8_t mac_addr[6];
    uint8_t status;
} virtio_nic_dev_t;

bool virtio_nic_init(net_device_t *net_dev);

bool virtio_nic_irq_handler();


#endif