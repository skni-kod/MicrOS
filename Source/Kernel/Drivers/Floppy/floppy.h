#ifndef FLOPPY_H
#define FLOPPY_H

#define DEVICE_NUMBER 0
#define IDLE_TIME 3000

#define DMA_SINGLE_CHANNEL_MASK_REGISTER 0x0A
#define DMA_FLIP_FLOP_RESET_REGISTER 0xD8
#define DMA_START_ADDRESS_REGISTER 0x04
#define DMA_COUNT_REGISTER_CHANNEL 0x05
#define DMA_EXTERNAL_PAGE_REGISTER 0x81
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
#include "../../../../Library/string.h"
#include "../../Logger/logger.h"
#include "../../Timer/timer.h"

void floppy_init();
void floppy_lba_to_chs(uint8_t lba, uint8_t *head, uint8_t *track, uint8_t *sector);
uint8_t floppy_reset();
uint8_t floppy_wait_until_ready();
uint8_t floppy_send_command(uint8_t cmd);
uint8_t floppy_read_data();
void floppy_confirm_interrupt(uint32_t* st0, uint32_t* cylinder);
void floppy_set_parameters(uint32_t step_rate, uint32_t head_load_time, uint32_t head_unload_time, bool dma);
uint8_t floppy_calibrate();
uint8_t* floppy_read_sector(uint8_t sector);
void floppy_write_sector(uint8_t sector, uint8_t* content);
uint8_t* floppy_do_operation_on_sector(uint8_t head, uint8_t track, uint8_t sector, bool read);
void floppy_enable_motor();
void floppy_disable_motor();
int floppy_seek(uint32_t cylinder, uint32_t head);

void floppy_dma_init(bool read);

void floppy_wait_for_interrupt();
void floppy_interrupt();
void floppy_timer_interrupt();

#endif