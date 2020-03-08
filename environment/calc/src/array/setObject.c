#define _CRT_SECURE_NO_WARNINGS
#define _USE_MATH_DEFINES
#include "../array.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* Set new data for object instance */
void setObject(Object* object, const void* data, const Type type) {

	free(object->Data);
	object->Type = type;
	object->Length = getDataSize(data, type);
	object->Data = malloc(object->Length);
	if (object->Data != NULL) {
		memcpy(object->Data, data,object->Length);
	}
	else {
		exit(-1);
	}
}
