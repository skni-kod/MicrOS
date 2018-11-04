#ifndef FLOPPY_H
#define FLOPPY_H

#define DMA_SINGLE_CHANNEL_MASK_REGISTER 0x0A
#define DMA_FLIP_FLOP_RESET_REGISTER 0xD8
#define DMA_START_ADDRESS_REGISTER 0x04
#define DMA_COUNT_REGISTER_CHANNEL 0x05
#define DMA_EXTERNAL_PAGE_REGISTER 0x80
#define DMA_MODE_REGISTER 0x0B

#define FLOPPY_DIGITAL_OUTPUT_REGISTER 0x3f2
#define FLOPPY_MAIN_STAUTS_REGISTER 0x3f4
#define FLOPPY_DATA_REGISTER 0x3f5
#define FLOPPY_CONTROL_REGISTER 0x3f7

#include <stdint.h>
#include <stdbool.h>
#include "floppy_header.h"
#include "../../Interrupts/PIC/pic.h"
#include "../../Assembly/io.h"

void floppy_init();
void floppy_reset();

void floppy_dma_init();
void floppy_dma_read();

void floppy_wait_for_interrupt();
void floppy_interrupt();

#endif