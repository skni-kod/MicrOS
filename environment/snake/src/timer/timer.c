#include "timer.h"

unsigned int start_point;

void timer_reset()
{
    start_point = micros_timer_get_system_clock();
}

void timer_display(int x, int y)
{
    int elapsed_seconds = (micros_timer_get_system_clock() - start_point) / 1000;
    
    micros_console_position cursor_position; 
    cursor_position.x = x;
    cursor_position.y = y;
    
    micros_console_set_cursor_position(&cursor_position);
    
    printf("%02d:%02d", elapsed_seconds / 60, elapsed_seconds % 60);
}