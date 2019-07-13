#ifndef HIGHSCORES_CONTAINER_H
#define HIGHSCORES_CONTAINER_H

#define HIGHSCORES_ENTRIES_COUNT 5

#include "highscores_entry.h"

typedef struct highscores_container
{
    highscores_entry entries[HIGHSCORES_ENTRIES_COUNT];    
} highscores_container;

#endif