#ifndef OBJ_H
#define OBJ_H

#include "../lib/vector.h"
#include "../lib/geometry.h"

typedef struct _obj_model
{
    vector vertices;
    vector indices;
    vector uv;
} obj_model;

obj_model* load_obj_model(char* filename);

#endif