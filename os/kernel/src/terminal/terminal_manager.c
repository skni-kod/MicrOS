#include "terminal_manager.h"
#include "memory/heap/heap.h"
#include "drivers/dal/videocard/videocard.h"
#include "process/manager/process_manager.h"

uint32_t active_terminal_id = 0;
uint32_t next_terminal_id = 0;
terminal_struct* terminals_array = NULL;
uint32_t terminal_number = 0;

int8_t create_terminal()
{
    void* ptr = heap_kernel_realloc(terminals_array, (terminal_number + 1) * sizeof(terminal_struct), 0);
    if(ptr == NULL) return -1;
    terminals_array = ptr;
    terminals_array[terminal_number].terminal_id = next_terminal_id;
    terminals_array[terminal_number].cursor_position_x = 0;
    terminals_array[terminal_number].cursor_position_y = 0;
    terminals_array[terminal_number].screen_mode = 0x03;
    terminals_array[terminal_number].screen_buffer = video_card_create_external_buffer(0x03);
    uint32_t process_id = process_manager_create_process("A:/ENV/SHELL.ELF", "", 1000, false);
    terminals_array[terminal_number].active_process = process_manager_get_process_info(process_id);
    void process_manager_set_active_process_id(process_id);
    active_terminal_id = next_terminal_id;
    terminal_number++;
    next_terminal_id++;
    return 0;
}

int8_t destroy_terminal(uint32_t terminal_id)
{
    return -1;
}

int8_t destroy_active_terminal()
{
    return -1;
}

//TODO: Functions for writting to chars and pixels, changing modes, changing active terminal and get list of terminals