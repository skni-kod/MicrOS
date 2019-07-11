#ifndef BOARD_H
#define BOARD_H

#include <micros.h>
#include <stdio.h>
#include "board_state.h"
#include "../vector/vector.h"
#include "../helpers/point.h"
#include "../helpers/direction.h"

void board_init(int width, int height);
bool board_logic();
void board_input();
void board_draw();
board_state board_get_state();

void board_generate();
void board_generate_snake();
void board_generate_food(int count);

#endif