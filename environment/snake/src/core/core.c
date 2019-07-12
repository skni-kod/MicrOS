#include "core.h"

scene_type current_scene_type;

scene scenes[SCENES_COUNT] =
{
    {
        .type = scene_type_main_menu,
        .scene_init = menu_init,
        .scene_input = menu_input,
        .scene_logic = menu_logic,
        .scene_draw = menu_draw,
        .scene_delete = menu_delete
    },
    {
        .type = scene_type_game,
        .scene_init = board_init,
        .scene_input = board_input,
        .scene_logic = board_logic,
        .scene_draw = board_draw,
        .scene_delete = board_delete
    },
    {
        .type = scene_type_highscores
    },
    {
        .type = scene_type_about
    },
    {
        .type = scene_type_exit
    }
};

void core_run()
{
    micros_process_set_current_process_name("SNAKE");
    micros_console_set_cursor_visibility(false);
    
    current_scene_type = scene_type_main_menu;
    scenes[current_scene_type].scene_init();
    
    while(1)
    {
        scenes[current_scene_type].scene_input();
        
        scene_type requested_scene = scenes[current_scene_type].scene_logic();
        if(requested_scene != scene_type_none)
        {
            scenes[current_scene_type].scene_delete();
            current_scene_type = requested_scene;
            
            scenes[current_scene_type].scene_init();
            continue;
        }
        
        scenes[current_scene_type].scene_draw();
        micros_process_current_process_sleep(10);
    }
    
    /*while(1)
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
    }*/
}

/*void game_display_lost_splashscreen()
{
    micros_console_position cursor_position;
            
    cursor_position.x = 0;
    cursor_position.y = 0;
    micros_console_set_cursor_position(&cursor_position);
    
    micros_console_set_background_color(micros_console_color_black);
    micros_console_set_foreground_color(micros_console_color_light_gray);
    
    micros_console_clear();
    printf(splashscreen_lost, board_get_eaten_food());
}*/