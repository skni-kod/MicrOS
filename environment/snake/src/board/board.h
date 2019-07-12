#ifndef BOARD_H
#define BOARD_H

#include <micros.h>
#include <stdio.h>
#include "board_state.h"
#include "../vector/vector.h"
#include "../helpers/point.h"
#include "../helpers/direction.h"

void board_init(int _width, int height, int _new_food_count, int _generate_food_edge, float _acceleration_ratio);
bool board_logic();
void board_input();
void board_draw();
board_state board_get_state();
int board_get_eaten_food();

void board_generate();
void board_generate_snake();
void board_generate_food();

#endif