#include "interface.h"

void interface_display(int line, int score, int health)
{
    micros_console_position cursor_position;
    cursor_position.x = 0;
    cursor_position.y = line;
    
    micros_console_set_cursor_position(&cursor_position);
    micros_console_set_background_color(micros_console_color_black);
    micros_console_set_foreground_color(micros_console_color_light_gray);
    
    printf("                 Score: %d                         Health: %d", score, health);
}