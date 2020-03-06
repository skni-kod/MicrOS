#define _CRT_SECURE_NO_WARNINGS
#define _USE_MATH_DEFINES
#include "../array.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* Initialize empty array */
void initArray(Array* array) {
    array->Size = 0;
    array->Data = malloc(sizeof(void*));
}
