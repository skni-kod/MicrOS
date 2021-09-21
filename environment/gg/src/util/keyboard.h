#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "../stdafx.h"
#include <micros.h>

typedef struct input
{
    bool lastFrame[key_f12];
    bool currentFrame[key_f12];
} input;


void initInput();

//Process inputs, might crash
void processInput();

bool getKeyDown(micros_keyboard_keys key);
bool getKeyUp(micros_keyboard_keys key);
bool getKey(micros_keyboard_keys key);

#endif