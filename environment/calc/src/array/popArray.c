#define _CRT_SECURE_NO_WARNINGS
#include "../array.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* Pop last from array */
void popArray(Array* array) {

	if (array == NULL || array->Size == 0) {
		return;
	}

	array->Size -= 1;

	free(((Object*)array->Data[array->Size])->Data);
	free(array->Data[array->Size]);

	void* tmp = array->Data;
	array->Data = realloc(array->Data, (sizeof(void*) * (array->Size)));
	if (array->Data != NULL) {
		return;
	}
	else {
		array->Data = tmp;
		exit(-1);
	}
}