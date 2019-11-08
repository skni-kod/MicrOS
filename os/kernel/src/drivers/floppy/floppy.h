#ifndef FLOPPY_H
#define FLOPPY_H

#define FLOPPY_HEADER_DATA 0xc0007c00

#define DEVICE_NUMBER 0
#define IDLE_TIME 3000

#define FLOPPY_DIGITAL_OUTPUT_REGISTER 0x3f2
#define FLOPPY_MAIN_STAUTS_REGISTER 0x3f4
#define FLOPPY_DATA_REGISTER 0x3f5
#define FLOPPY_CONTROL_REGISTER 0x3f7

#include <stdint.h>
#include <stdbool.h>
#include "floppy_header.h"
#include "cpu/pic/pic.h"
#include "cpu/idt/idt.h"
#include "assembly/io.h"
#include <stdlib.h>
#include <string.h>
#include "logger/logger.h"
#include "cpu/timer/timer.h"
#include "cpu/dma/dma.h"
#include <ktime.h>

bool floppy_init();
void floppy_lba_to_chs(uint16_t lba, uint8_t *head, uint8_t *track, uint8_t *sector);
bool floppy_reset();
bool floppy_wait_until_ready();
bool floppy_send_command(uint8_t cmd);
uint8_t floppy_read_data();
void floppy_confirm_interrupt(uint8_t *st0, uint8_t *cylinder);
void floppy_set_parameters(uint32_t step_rate, uint32_t head_load_time, uint32_t head_unload_time, bool dma);
bool floppy_calibrate();
uint8_t *floppy_read_sector(uint16_t sector);
void floppy_write_sector(uint16_t sector, uint8_t *content);
uint8_t *floppy_do_operation_on_sector(uint8_t head, uint8_t track, uint8_t sector, bool read);
void floppy_enable_motor();
void floppy_disable_motor();
bool floppy_seek(uint8_t cylinder, uint8_t head);

void floppy_wait_for_interrupt();
void floppy_interrupt();
void floppy_timer_interrupt();

#endif