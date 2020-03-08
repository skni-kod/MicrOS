#define _CRT_SECURE_NO_WARNINGS
#define _USE_MATH_DEFINES
#include "../array.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* Initialize empty array */
Array* initArray(void) {
    Array* array = 0;
    array = malloc(sizeof(Array));

    if (array != NULL) {
        array->Size = 0;
        array->Data = malloc(sizeof(void*));
        return array;
    }
}
