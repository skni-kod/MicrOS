#ifndef MENU_H
#define MENU_H

#include <micros.h>
#include <stdio.h>
#include "menu_button.h"
#include "../scene_type.h"
#include "../../../helpers/point.h"

void menu_init();
void menu_input();
scene_type menu_logic();
void menu_draw();
void menu_delete();

#endif