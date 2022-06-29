/*
    @JakubPrzystasz
    Created: 28.07.2021
    Modify: 28.07.2021
*/
#ifndef VIRTIO_NET_PCI_H
#define VIRTIO_NET_PCI_H

#include "../../pci/pci.h"
#include "../../../assembly/io.h"
#include "../../../cpu/idt/idt.h"
#include "../../serial/serial.h"
#include "../../../network/network_definitions.h"
#include "../../../klibrary/kvector.h"
#include "../../../debug_helpers/library/kernel_stdio.h"
#include "../../virtio/virtio-pci.h"
#include "../../../logger/logger.h"
#include <stdlib.h>

/* Device specification for PCI (4.1.1) */
#define VIRTIO_NET_DEVICE_VENDOR_ID 0x1AF4
#define VIRTIO_NET_DEVICE_SUBSYSTEM_ID 0x1
#define VIRTIO_NET_DEVICE_ID_BEGIN 0x1000
#define VIRTIO_NET_DEVICE_ID_END 0x103F
#define VIRTIO_NET_DEVICE_NAME "VirtIO NIC"
#define VIRTIO_NET_MTU 1526

/* Feature bits (5.1.3) */
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
/*VIRTIO_NET_F_GUEST_TSO6 (8) Driver can receive TSOv6. */
#define VIRTIO_NET_F_GUEST_TSO6 (1 << 8)
/*VIRTIO_NET_F_GUEST_ECN (9) Driver can receive TSO with ECN.*/
#define VIRTIO_NET_F_GUEST_ECN (1 << 9)
/*VIRTIO_NET_F_GUEST_UFO (10) Driver can receive UFO.*/
#define VIRTIO_NET_F_GUEST_UFO (1 << 10)
/*VIRTIO_NET_F_HOST_TSO4 (11) Device can receive TSOv4.*/
#define VIRTIO_NET_F_HOST_TSO4 (1 << 11)
/*VIRTIO_NET_F_HOST_TSO6 (12) Device can receive TSOv6.*/
#define VIRTIO_NET_F_HOST_TSO6 (1 << 12)
/*VIRTIO_NET_F_HOST_ECN (13) Device can receive TSO with ECN.*/
#define VIRTIO_NET_F_HOST_ECN (1 << 13)
/*VIRTIO_NET_F_HOST_UFO (14) Device can receive UFO.*/
#define VIRTIO_NET_F_HOST_UFO (1 << 14)
/*VIRTIO_NET_F_MRG_RXBUF (15) Driver can merge receive buffers.*/
#define VIRTIO_NET_F_MRG_RXBUF (1 << 15)
/*VIRTIO_NET_F_STATUS (16) Configuration status field is available.*/
#define VIRTIO_NET_F_STATUS (1 << 16)
/*VIRTIO_NET_F_CTRL_VQ (17) Control channel is available.*/
#define VIRTIO_NET_F_CTRL_VQ (1 << 17)
/*VIRTIO_NET_F_CTRL_RX (18) Control channel RX mode support.*/
#define VIRTIO_NET_F_CTRL_RX (1 << 18)
/*VIRTIO_NET_F_CTRL_VLAN (19) Control channel VLAN filtering.*/
#define VIRTIO_NET_F_CTRL_VLAN (1 << 19)
/*VIRTIO_NET_F_GUEST_ANNOUNCE(21) Driver can send gratuitous packets.*/
#define VIRTIO_NET_F_GUEST_ANNOUNCE (1 << 21)
/*VIRTIO_NET_F_MQ(22) Device supports multiqueue with automatic receive steering.*/
#define VIRTIO_NET_F_MQ (1 << 22)
/*VIRTIO_NET_F_CTRL_MAC_ADDR(23) Set MAC address through control channel.*/
#define VIRTIO_NET_F_CTRL_MAC_ADDR (1 << 23)
/*VIRTIO_NET_F_RSC_EXT(61) Device can process duplicated ACKs and report number of coalesced segments
and duplicated ACKs */
#define VIRTIO_NET_F_RSC_EXT (1 << 61)
/*VIRTIO_NET_F_STANDBY(62) Device may act as a standby for a primary device with the same MAC address*/
#define VIRTIO_NET_F_STANDBY (1 << 62)

/* Legacy interface feature bits (5.1.3.2)*/
/*VIRTIO_NET_F_GSO (6) Device handles packets with any GSO type. This was supposed to indicate segmentation
offload support, but upon further investigation it became clear that multiple bits were needed.*/
#define VIRTIO_NET_F_GSO (1 << 6)
/*VIRTIO_NET_F_GUEST_RSC4 (41) Device coalesces TCPIP v4 packets. This was implemented by hypervisor
patch for certification purposes and current Windows driver depends on it. It will not function
if virtio-net device reports this feature.*/
#define VIRTIO_NET_F_GUEST_RSC4 (1 << 41)
/*VIRTIO_NET_F_GUEST_RSC6 (42) Device coalesces TCPIP v6 packets. Similar to VIRTIO_NET_F_-GUEST_RSC4.*/
#define VIRTIO_NET_F_GUEST_RSC6 (1 << 42)

// NIC specific registers 
#define VIRTIO_NET_REGISTER_MAC_1 0x14
#define VIRTIO_NET_REGISTER_MAC_2 0x15
#define VIRTIO_NET_REGISTER_MAC_3 0x16
#define VIRTIO_NET_REGISTER_MAC_4 0x17
#define VIRTIO_NET_REGISTER_MAC_5 0x18
#define VIRTIO_NET_REGISTER_MAC_6 0x19
#define VIRTIO_NET_REGISTER_STATUS 0x1A

/* Device specific queues indexes */
#define VIRTIO_NET_RECEIVE_QUEUE_INDEX 0
#define VIRTIO_NET_TRANSMIT_QUEUE_INDEX 1
#define VIRTIO_NET_CONTROL_QUEUE_INDEX 2



#undef REQUIRED_FEATURES
#define REQUIRED_FEATURES (/*VIRTIO_NET_F_CSUM |*/ VIRTIO_NET_F_MAC | VIRTIO_NET_F_MRG_RXBUF)

// Flags for virtio_net_hdr
#define VIRTIO_NET_HDR_F_NEEDS_CSUM 1
#define VIRTIO_NET_HDR_F_DATA_VALID 2
#define VIRTIO_NET_HDR_F_RSC_INFO 4

// Values for virtio_net_hdr gso_type
#define VIRTIO_NET_HDR_GSO_NONE 0
#define VIRTIO_NET_HDR_GSO_TCPV4 1
#define VIRTIO_NET_HDR_GSO_UDP 3
#define VIRTIO_NET_HDR_GSO_TCPV6 4
#define VIRTIO_NET_HDR_GSO_ECN 0x80

//
#define VIRTIO_NET_HEADER_SIZE sizeof(virtio_nic_net_header)

typedef struct
{
    uint8_t flags;
    uint8_t segmentation_offload;
    uint16_t header_size;
    uint16_t segment_size;
    uint16_t checksum_start;
    uint16_t checksum_offset;
    uint16_t buffers_count; 
} virtio_nic_net_header;

typedef struct
{
    uint8_t bar_type;
    uint16_t io_base;
    uint32_t mem_base;
    uint8_t mac_addr[6];
    uint8_t status;
} virtio_nic_dev;

bool virtio_nic_init(net_device_t *net_dev);

bool virtio_nic_irq_handler();

void virtio_nic_send(nic_data_t *data);

void virtio_nic_init_queue(virtq *virtqueue, uint16_t queueIndex);

void virtio_nic_receive();

void virtio_nic_setup_buffers(uint16_t buffers_count);

void virtio_nic_get_mac(uint8_t* buffer);

uint32_t virtio_nic_reg_read(uint16_t reg);

void virtio_nic_reg_write(uint16_t reg, uint32_t data);

#endif