// #include "packet_queue.h"

// void packet_queue_init()
// {
//     rx_queue = heap_kernel_alloc(sizeof(kvector), 0);
//     tx_queue = heap_kernel_alloc(sizeof(kvector), 0);
//     kvector_init(rx_queue);
//     kvector_init(tx_queue);
// }

// void net_add_rx(uint32_t packet_length, uint32_t packet_data, uint32_t mac)
// {
//     net_packet_t *packet = heap_kernel_alloc(sizeof(packet), 0);
//     packet->packet_data = packet_data;
//     packet->packet_length = packet_length;
//     memcpy(packet->device_mac, *(uint8_t *)mac, sizeof(uint8_t) * 6);
//     kvector_add(rx_queue, packet);
//     return;
// }