#ifndef FLOPPY_H
#define FLOPPY_H

#include <stdint.h>
#include "floppy_header.h"
#include "../../Assembly/io.h"

void floppy_init();
void floppy_init_dma();

#endif