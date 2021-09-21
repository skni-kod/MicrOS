#include "keyboard.h"

input* inputPtr;

void initInput()
{
    inputPtr = (input*)calloc(1, sizeof(input));
}

void processInput()
{
    if(micros_keyboard_is_key_pressed())
    {
        micros_keyboard_scan_ascii_pair pressed_key;
        micros_keyboard_get_pressed_key(&pressed_key);
    }

    //THIS IS BAD!!!
    //But currently no other way to tell which value is "KEY MAX"
    memcpy(inputPtr->lastFrame, inputPtr->currentFrame, sizeof(bool)*key_f12);

    for(int i = 1; i <= key_f12; i++)
    {
        inputPtr->currentFrame[i-1] = micros_keyboard_get_key_state(i);
    }
}

bool getKeyDown(micros_keyboard_keys key)
{
    return inputPtr->currentFrame[key-1] && !inputPtr->lastFrame[key-1];
}

bool getKeyUp(micros_keyboard_keys key)
{
    return !inputPtr->currentFrame[key-1] && inputPtr->lastFrame[key-1];
}
bool getKey(micros_keyboard_keys key)
{
    return inputPtr->currentFrame[key-1];
}
