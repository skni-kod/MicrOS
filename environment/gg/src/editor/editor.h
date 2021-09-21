#ifndef EDITOR_H
#define EDITOR_H

#include "../tile/tilemap.h"

void runEditor();

void tickCursor();

void updateCamera();

void placeTile(int x, int y, int layer, tileID id, tilemap* map);

void deleteTile(int x, int y, int layer, tilemap* map);

#endif