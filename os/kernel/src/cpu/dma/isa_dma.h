#ifndef ISA_DMA_H
#define ISA_DMA_H

#include <stdbool.h>
#include "assembly/io.h"

#define ISA_DMA_SINGLE_CHANNEL_MASK_REGISTER 0x0A
#define ISA_DMA_FLIP_FLOP_RESET_REGISTER 0xD8
#define ISA_DMA_START_ADDRESS_REGISTER 0x04
#define ISA_DMA_COUNT_REGISTER_CHANNEL 0x05
#define ISA_DMA_EXTERNAL_PAGE_REGISTER 0x81
#define ISA_DMA_MODE_REGISTER 0x0B

void isa_dma_init_transfer(uint8_t channel, bool read);

#endif