#ifndef MICROS_MOUSE_H
#define MICROS_MOUSE_H 

#include "../../assembly/io.h"
#include "../../logger/logger.h"
#include "../../cpu/idt/idt.h"
#include "../../cpu/timer/timer.h"
#include "../serial/serial.h"
#include <stdlib.h>
#include <stdio.h>

//irq prioryty in system (probably)
#define IRQ_NUM 12

//TIMEOUTS
#define TIMEOUT 100000
#define PS2_MOUSE_TIMEOUT 10000
#define DOUBLE_CLICK_TIME 800 //it is sample value, it can be change

//ps2 mouse commands
#define PS2_MOUSE_CMD_RESET 0xFF
#define PS2_MOUSE_CMD_RESEND 0xFE
#define PS2_MOUSE_CMD_DEFAULT 0xF6
#define PS2_MOUSE_CMD_DISABLE_REPORT 0xF5
#define PS2_MOUSE_CMD_ENABLE_REPORT 0xF4
#define PS2_MOUSE_CMD_GET_DEVICE_ID 0xF2
#define PS2_MOUSE_CMD_ACK 0xFA
#define PS2_MOUSE_CMD_I_WANT_TO_SEND_COMMAND 0xD4
#define PS2_MOUSE_CMD_GIVE_ME_COMPAQ_STATUS 0x20
#define PS2_MOUSE_CMD_I_WANT_TO_SET_COMPAQ_STATUS 0x60
#define PS2_MOUSE_CMD_ENABLE_SECOND_PORT 0xA8
#define PS2_MOUSE_CMD_SET_SAMPLE_RATE 0xF3 

//PS2 Mouse ports
#define PS2_DATA_PORT    0x60
#define PS2_CMD_PORT     0x64

//bitwise operators
#define PS2_OUTPUT_BUFFER_STATUS     (1 << 0)
#define PS2_INPUT_BUFFER_STATUS      (1 << 1)

//mouse structure - all buttons, scroll and position of cursor
typedef struct PS2_Mouse_Parameters{
    uint32_t mouse_x;
    uint32_t  mouse_y;
    uint32_t  mouse_z_horizontal;
    uint32_t mouse_z_vertical;
    uint32_t last_click_time;
    bool last_click_was_left;
    bool left_click;
    bool right_click;
    bool middle_click;
    bool additional_button_1;
    bool additional_button_2;
    bool double_click;
} PS2_Mouse_Parameters;

//Full means that something is on the mouse port, while Empty that the port is empty
typedef enum BufferStatus {
    Full = 1,
    Empty = 0
} BufferStatus;

//Input - mouse waits for command, Output - mouse sends command
typedef enum BufferType {
    Input = 1,
    Output = 0
} BufferType;

//using in initialization of mouse - to enable scroll and additional butons
typedef enum SequenceType {
    ScrollEnable = 0,
    AdditionalButonsEnable = 1
} SequenceType;

//      FUNCTIONS

//ps2 mouse inicjalization function. This function have to be before keyboard initialization in kernel
void ps2mouse_init();

//scroll initialization
void ps2mouse_scroll_init(SequenceType type);

//ps2 mouse irq handler function
bool ps2mouse_irq_handler();

//ps2 mouse function checking if we can send a byte to the mouse and if we can get a byte from the mouse
BufferStatus ps2mouse_check_buffer_status(BufferType type);

//ps2 mouse function to send a byte to mouse
bool ps2mouse_write(uint8_t byte);

//ps2 mouse function to get a byte from mouse
bool ps2mouse_read(uint8_t* byte);

//functions to settinng and getting mouse parameters
void ps2mouse_get_cursor_position(int32_t* x, int32_t* y);
void ps2mouse_set_cursor_position(int32_t x, int32_t y);
void ps2mouse_set_standard_buttons(uint8_t byte);
void ps2mouse_set_additional_buttons(uint8_t byte);
void ps2mouse_set_scroll(int32_t mouse_z_v, int32_t mouse_z_h);
bool ps2mouse_get_middle();
bool ps2mouse_get_right();
bool ps2mouse_get_left();
bool ps2mouse_get_add_b4();
bool ps2mouse_get_add_b5();
uint16_t ps2mouse_get_scroll_h();
uint16_t ps2mouse_get_scroll_v();

//display functions
void double_click_display();
void right_click_display();
void left_click_display();
void middle_click_display();
void add1_click_display();
void add2_click_display();
void scroll_vertical_display();
void scroll_horizontal_display();
void x_display();
void y_display();

#endif

