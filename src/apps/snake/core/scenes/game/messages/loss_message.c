#include "loss_message.h"

const char *loss_message_screen =
"\n"
"\n"
"\n"
"\n"
"\n"
"                  __     __               _               _   \n" 
"                  \\ \\   / /              | |             | |  \n" 
"                   \\ \\_/ ___  _   _      | |     ___  ___| |_ \n"
"                    \\   / _ \\| | | |     | |    / _ \\/ __| __|\n"
"                     | | (_) | |_| |     | |___| (_) \\__ | |_ \n"
"                     |_|\\___/ \\__,_|     |______\\___/|___/\\__|\n"
"\n"
"\n"
"                                  Your score: %d\n"
"                              Press ENTER to return\n";

void loss_message_draw_and_wait(int score)
{
    micros_console_clear();
    
    micros_console_position cursor_position;
    cursor_position.x = 0;
    cursor_position.y = 0;
    micros_console_set_cursor_position(&cursor_position);
    
    micros_console_set_background_color(micros_console_color_black);
    micros_console_set_foreground_color(micros_console_color_light_gray);
    
    printf(loss_message_screen, score);
    
    micros_keyboard_scan_ascii_pair pressed_key;
    
    while(pressed_key.scancode != key_enter)
    {
        micros_keyboard_wait_for_key_press(&pressed_key);
    }
}