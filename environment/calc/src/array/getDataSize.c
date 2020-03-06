#define _CRT_SECURE_NO_WARNINGS
#define _USE_MATH_DEFINES
#include "../array.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* Returns length of pointed data */
size_t getDataSize(const void* data, Type type) {

    switch (type) {
    case String:
        return (sizeof(char) * (strlen(data) + 1));
    case Int:
        return (sizeof(int));
    case Double:
        return (sizeof(double));
    case Pointer:
        return (sizeof(void*));
    default:
        return -1;
    }
}
