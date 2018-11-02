#pragma once
#define MASTER_PIC_COMMAND 0x20
#define MASTER_PIC_DATA 0x21

#define SLAVE_PIC_COMMAND 0xA0
#define SLAVE_PIC_DATA 0xA1

#include <stdint.h>

void pic_init();
void io_wait();