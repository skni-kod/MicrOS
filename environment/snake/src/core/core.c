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
        .scene_init = game_init,
        .scene_input = game_input,
        .scene_logic = game_logic,
        .scene_draw = game_draw,
        .scene_delete = game_delete
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
}