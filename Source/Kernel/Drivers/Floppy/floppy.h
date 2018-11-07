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
#include "../../../../Library/stdlib.h"

void floppy_init();
void floppy_reset();
void floppy_wait_until_ready();
void floppy_send_command(uint8_t cmd);
uint8_t floppy_read_data();
void floppy_get_interrupt_data(uint32_t* status_register, uint32_t* cylinder);
void floppy_set_parameters(uint32_t step_rate, uint32_t head_load_time, uint32_t head_unload_time, bool dma);
void floppy_calibrate();
void floppy_enable_motor();
void floppy_disable_motor();

void floppy_dma_init();
void floppy_dma_read();

void floppy_wait_for_interrupt();
void floppy_interrupt();

#endif