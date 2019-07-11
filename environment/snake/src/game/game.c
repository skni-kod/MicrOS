#include "game.h"

void game_run()
{
    micros_process_set_current_process_name("SNAKE");
    micros_console_set_cursor_visibility(false);
    micros_console_clear();

    board_init(60, 20);
    board_generate_food(10);
    
    while(1)
    {
        if(board_logic())
        {
            board_draw();
        }
        
        micros_process_current_process_sleep(1000);
    }
}