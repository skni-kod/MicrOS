#define _CRT_SECURE_NO_WARNINGS
#define _USE_MATH_DEFINES
#include "../array.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* Initialize new instance of object */
Object* initObject(const void* data, const Type type){
    
    Object* obj = 0;
    obj = malloc(sizeof(Object));
    if (obj != NULL) {
        obj->Type = type;
        obj->Length = getDataSize(data, type);
        obj->Data = malloc(obj->Length);
        if (obj->Data != NULL) {
            memcpy(obj->Data, data, obj->Length);
            return obj;
        }
        else{
            exit(-1);
        }
    }
    else {
        exit(-1);
    }
}
