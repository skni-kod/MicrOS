#ifndef FLOPPY_H
#define FLOPPY_H

#include <stdint.h>
#include <stdbool.h>
#include "floppy_header.h"
#include "../../Interrupts/PIC/pic.h"
#include "../../Assembly/io.h"

void floppy_init();
void floppy_init_dma();
void floppy_reset();
void floppy_dma_read();

void floppy_wait_for_interrupt();
void floppy_interrupt();

#endif