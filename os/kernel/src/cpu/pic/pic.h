#ifndef PIC_H
#define PIC_H

#define MASTER_PIC_COMMAND 0x20
#define MASTER_PIC_DATA 0x21

#define SLAVE_PIC_COMMAND 0xA0
#define SLAVE_PIC_DATA 0xA1

#include <stdint.h>
#include "assembly/io.h"

void pic_init();
void pic_remap(uint32_t master_offset, uint32_t slave_offset);
void pic_enable_irq(uint8_t interrupt_number);
void pic_disable_irq(uint8_t interrupt_number);
void pic_confirm_master();
void pic_confirm_master_and_slave();

#endif