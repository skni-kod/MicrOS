#define _CRT_SECURE_NO_WARNINGS
#include "../array.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* Append value to array */
void appendToArray(Array* array, const Object* data) {

	if (data == NULL) {
		return;
	}

	if (array == NULL) {
		array = initArray();
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

	/* Reallocate space for pointers */
	void* tmp = array->Data;
	array->Data = realloc(array->Data, sizeof(void*) * (array->Size + 1));
	
	if (array->Data != NULL) {
		/* Allocate space for new data */
		array->Data[array->Size] = malloc(sizeof(void*));
		if (array->Data[array->Size] != 0) {
			/* Copy pointer of new value stored in *obj */
			array->Data[array->Size] = obj;
			array->Size += 1;
		}
		else {
			exit(-1);
		}
	}
	else {
		array->Data = tmp;
		exit(-1);
	}

}
