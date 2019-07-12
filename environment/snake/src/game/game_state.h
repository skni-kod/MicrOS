#ifndef GAME_STATE_H
#define GAME_STATE_H

typedef enum game_state
{
    game_state_none,
    
    game_state_premenu,
    game_state_menu,
    
    game_state_pregame,
    game_state_game
} game_state;

#endif