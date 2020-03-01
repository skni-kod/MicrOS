#ifndef TERMINAL_MANAGER_H
#define TERMINAL_MANAGER_H

#include <stdint.h>
#include "../process/manager/process_info.h"

typedef struct terminal_struct
{
    uint32_t terminal_id;
    int8_t screen_mode;
    uint16_t cursor_position_x;
    uint16_t cursor_position_y;
    uint8_t* screen_buffer;
    process_info* active_process;

} terminal_struct;

#endif