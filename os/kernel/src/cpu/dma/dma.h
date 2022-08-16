#ifndef DMA_H
#define DMA_H

#include <stdbool.h>
#include "assembly/io.h"

#define DMA_SINGLE_CHANNEL_MASK_REGISTER 0x0A
#define DMA_FLIP_FLOP_RESET_REGISTER 0xD8
#define DMA_START_ADDRESS_REGISTER 0x04
#define DMA_COUNT_REGISTER_CHANNEL 0x05
#define DMA_EXTERNAL_PAGE_REGISTER 0x81
#define DMA_MODE_REGISTER 0x0B

void dma_init(uint32_t buffer_address);
void dma_init_transfer(uint8_t channel, bool read, uint16_t dataLen);
uint8_t *dma_get_buffer();

#endif