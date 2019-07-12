#include "game.h"

void game_run()
{
    micros_process_set_current_process_name("SNAKE");
    micros_console_set_cursor_visibility(false);
    micros_console_clear();

    board_init(60, 20, 10, 8, 0.1f);
    
    while(1)
    {
        board_input();
        if(board_logic())
        {
            board_draw();
        }
        
        if(board_get_state() == board_state_loss)
        {
            micros_console_position cursor_position;
            
            cursor_position.x = 0;
            cursor_position.y = 0;
            micros_console_set_cursor_position(&cursor_position);
            
            micros_console_set_background_color(micros_console_color_black);
            micros_console_set_foreground_color(micros_console_color_light_gray);
            
            micros_console_clear();
            micros_console_print_string(splashscreen_lost);
            
            while(1);
        }
        
        micros_process_current_process_sleep(10);
    }
}