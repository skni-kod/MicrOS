#include "game.h"

game_state state;

void game_run()
{
    state = game_state_premenu;
    
    micros_process_set_current_process_name("SNAKE");
    micros_console_set_cursor_visibility(false);
    
    while(1)
    {
        switch(state)
        {
            case game_state_premenu:
            {
                menu_init();
                state = game_state_menu;
            }
            
            case game_state_menu:
            {
                menu_input();
                if(menu_logic())
                {
                    menu_draw();
                }
                
                game_state requested_game_state = menu_get_requested_state();
                if(requested_game_state != game_state_none)
                {
                    state = requested_game_state;
                }
                
                break;
            }
            
            case game_state_pregame:
            {
                micros_console_clear();
                board_init(60, 20, 10, 8, 0.1f);
                timer_reset();
                
                state = game_state_game;
                break;
            }
            
            case game_state_game:
            {
                board_input();
                if(board_logic())
                {
                    board_draw();
                    interface_display(0, board_get_eaten_food(), 3);
                    timer_display(37, 23);
                }
                
                if(board_get_state() == board_state_loss)
                {
                    game_display_lost_splashscreen();
                    micros_keyboard_wait_for_key_press();
                    state = game_state_premenu;
                }
        
                break;
            }
        }
        
        micros_process_current_process_sleep(10);
    }
}

void game_display_lost_splashscreen()
{
    micros_console_position cursor_position;
            
    cursor_position.x = 0;
    cursor_position.y = 0;
    micros_console_set_cursor_position(&cursor_position);
    
    micros_console_set_background_color(micros_console_color_black);
    micros_console_set_foreground_color(micros_console_color_light_gray);
    
    micros_console_clear();
    printf(splashscreen_lost, board_get_eaten_food());
}