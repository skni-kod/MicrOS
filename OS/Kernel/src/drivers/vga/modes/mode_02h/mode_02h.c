#include "mode_02h.h"
#include "drivers/vga/modes/registerFunctions.h"
#include "memory/heap/heap.h"
#include "drivers/dal/videocard/videocard.h"
#include "drivers/vga/vga.h"

//DOUBLE BUFFER POINTER;
screen *mode02h_buffer = NULL;

int8_t mode02h_set_mode()
{
    return -1;
}

int8_t mode02h_turn_on_buffer()
{
    return -1;
}
int8_t mode02h_turn_off_buffer()
{
    return -1;
}
uint8_t mode02h_is_buffer_on()
{
    return -1;
}

int8_t mode02h_swap_buffers()
{
    return -1;
}

int8_t mode02h_draw_pixel(uint8_t color, uint16_t x, uint16_t y)
{
    return -1;
}
int8_t mode02h_draw_line(uint8_t color, uint16_t ax, uint16_t ay, uint16_t bx, uint16_t by)
{
    return -1;
}
int8_t mode02h_draw_circle(uint8_t color, uint16_t x, uint16_t y, uint16_t radius)
{
    return -1;
}
int8_t mode02h_draw_rectangle(uint8_t color, uint16_t ax, uint16_t ay, uint16_t bx, uint16_t by)
{
    return -1;
}
int8_t mode02h_clear_screen()
{
    return -1;
}

int8_t mode02h_draw_pixel_buffered(uint8_t color, uint16_t x, uint16_t y)
{
    return -1;
}
int8_t mode02h_draw_line_buffered(uint8_t color, uint16_t ax, uint16_t ay, uint16_t bx, uint16_t by)
{
    return -1;
}
int8_t mode02h_draw_circle_buffered(uint8_t color, uint16_t x, uint16_t y, uint16_t radius)
{
    return -1;
}
int8_t mode02h_draw_rectangle_buffered(uint8_t color, uint16_t ax, uint16_t ay, uint16_t bx, uint16_t by)
{
    return -1;
}
int8_t mode02h_clear_screen_buffered()
{
    return -1;
}