#ifndef HIGHSCORES_H
#define HIGHSCORES_H

#include <micros.h>
#include <string.h>
#include <stdio.h>
#include "highscores_container.h"
#include "../scene_type.h"
#include "../../../helpers/point.h"

void highscores_init();
void highscores_input();
scene_type highscores_logic();
void highscores_draw();
void highscores_delete();

void highscores_load_from_file();
void highscores_save_to_file();
int highscores_get_minimal_score_to_save();
void highscores_add_new_entry(const char *name, int score);

#endif