#ifndef MICROS_MOUSE_H
#define MICROS_MOUSE_H

#include "../../assembly/io.h"
#include "../../logger/logger.h"
#include "../../cpu/idt/idt.h"
#include "../serial/serial.h"
#include <stdlib.h>
#include <stdio.h>

#define IRQ_NUM 12
#define TIMEOUT 100000

#define PS2_MOUSE_CMD_RESET 0xFF
#define PS2_MOUSE_CMD_RESEND 0xFE
#define PS2_MOUSE_CMD_DEFAULT 0xF6
#define PS2_MOUSE_CMD_DISABLE_REPORT 0xF5
#define PS2_MOUSE_CMD_ENABLE_REPORT 0xF4
#define PS2_MOUSE_CMD_GET_DEVICE_ID 0xF2
#define PS2_MOUSE_CMD_ACK 0xFA
#define PS2_MOUSE_TIMEOUT 10000

struct Cursor {
    int cursor_x;
    int cursor_y;
};

typedef enum BufferStatus {
    Full = 1,
    Empty = 0
} BufferStatus;

typedef enum BufferType {
    Input = 1,
    Output = 0
} BufferType;

//! ps2mouse_init()
/*
    Initialization of the mouse
*/
void ps2mouse_init();
bool ps2mouse_irq_handler();
BufferStatus ps2mouse_check_buffer_status(BufferType type);
bool ps2mouse_write(uint8_t byte);
bool ps2mouse_read(uint8_t* byte);
void ps2mouse_get_cursor_position();
void ps2mouse_set_cursor_position();

#endif
