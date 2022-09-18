#ifndef game_H
#define game_H

#define EMPTY_FIELD_SYMBOL ' '
#define WALL_SYMBOL '#'
#define FOOD_SYMBOL '*'
#define SNAKE_SYMBOL 'S'

#include <micros.h>
#include <stdio.h>
#include "game_state.h"
#include "messages/loss_message.h"
#include "messages/new_highscore_message.h"
#include "interface/interface.h"
#include "timer/timer.h"
#include "../../../vector/vector.h"
#include "../../../helpers/point.h"
#include "../../../helpers/direction.h"
#include "../scene_type.h"
#include "../highscores/highscores.h"

void game_init();
void game_input();
scene_type game_logic();
void game_draw();
void game_delete();
int game_get_eaten_food();

void game_generate();
void game_generate_snake();
void game_generate_food();

void game_animate_death();
void game_set_snake_visibility(bool visible);
void game_reset_snake();

#endif