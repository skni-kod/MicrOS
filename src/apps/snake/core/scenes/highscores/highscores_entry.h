#ifndef HIGHSCORES_ENTRY_H
#define HIGHSCORES_ENTRY_H

#define HIGHSCORES_NAME_LENGTH 16

typedef struct highscores_entry
{
    char name[HIGHSCORES_NAME_LENGTH];
    int score;
} highscores_entry;

#endif