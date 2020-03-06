#define _CRT_SECURE_NO_WARNINGS
#define _USE_MATH_DEFINES
#include "../array.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* Delete data of object from memory */
void freeObject(Object* object) {
    free(((void*)object->Data));
    object->Type = 0;
    object->Length = 0;
}
