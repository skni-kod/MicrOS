#define _CRT_SECURE_NO_WARNINGS
#include "../array.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* Set new value */
void setData(Array* array, const Object* data, const unsigned int index) {

	if (data == NULL || array == NULL || (index) > (array->Size - 1)) {
		return;
	}

	/* Copy data object to new instance */
	Object* obj = 0;
	obj = malloc(sizeof(Object));
	if (obj != NULL) {
		obj->Type = data->Type;
		obj->Length = data->Length;

		obj->Data = malloc(obj->Length);
		if (obj->Data == NULL) {
			exit(-1);
		}
		memcpy(obj->Data, data->Data, obj->Length);
	}

	if (array->Data != NULL) {
		array->Data[index] = obj;
	}

}
