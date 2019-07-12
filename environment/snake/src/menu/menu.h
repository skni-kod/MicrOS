#ifndef MENU_H
#define MENU_H

#include <micros.h>
#include <stdio.h>
#include "menu_screens.h"
#include "menu_button.h"
#include "../helpers/point.h"
#include "../game/game_state.h"

void menu_init();
void menu_input();
bool menu_logic();
void menu_draw();

void menu_set_requested_state();
game_state menu_get_requested_state();

#endif