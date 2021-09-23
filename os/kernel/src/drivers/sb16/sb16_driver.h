#ifndef SB16_DRIVER_H
#define SB16_DRIVER_H

#include <stdint.h>
#include <stdbool.h>
#include "../cpu/idt/idt.h"

#define SB16_DSP_MIXER_PORT 0x224
#define SB16_DSP_MIXER_DATA_PORT 0x225
#define SB16_DSP_RESET_PORT 0x226
#define SB16_DSP_READ_PORT 0x22A
#define SB16_DSP_WRTIE_PORT 0x22C
#define SB16_DSP_READ_STATUS_PORT 0x22E
#define SB16_DSP_16_BIT_ACK_PORT 0x22F


void sb16_init();
bool sb16_is_present();
void sb16_write_dsp(uint8_t value);
uint8_t sb16_read_dsp();

void sb16_write_mixer_register(uint8_t index, uint8_t value);
uint8_t sb16_read_mixer_register(uint8_t index);

bool sb16_interrupt_handler(interrupt_state* state);

#endif