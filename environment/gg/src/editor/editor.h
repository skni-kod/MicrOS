#ifndef EDITOR_H
#define EDITOR_H

#include "../tile/tilemap.h"

void runEditor();

void tickCursor();

void updateCameraEditor();

void placeTile(int x, int y, int layer, tileID id, tilemap* map);

void deleteTile(int x, int y, int layer, tilemap* map);

void insertCollider(int x, int y, tilemap* map);

#endif