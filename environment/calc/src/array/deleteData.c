#define _CRT_SECURE_NO_WARNINGS
#include "../array.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* Delete specified value */
void deleteData(Array* array, const unsigned int index) {

	if (array == NULL || (index) > (array->Size - 1)) {
		return;
	}

	free(((Object*)array->Data[index])->Data);
	free(array->Data[index]);

	void* tmp = 0;

	for (unsigned int i = index; i+1 < array->Size; i++) {
		array->Data[i] = array->Data[i + 1];
	}

	array->Size -= 1;

	tmp = array->Data;
	array->Data = realloc(array->Data, (sizeof(void*) * (array->Size)));
	
	if (array->Data == NULL) {
		array->Data = tmp;
		exit(-1);
	}
}