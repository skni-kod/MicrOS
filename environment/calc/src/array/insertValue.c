#define _CRT_SECURE_NO_WARNINGS
#include "../array.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* Insert value to array */
void insertValue(Array* array, const Object* data, const unsigned int index) {

	if (data == NULL || array == NULL || (index) > (array->Size - 1)) {
		return;
	}

	appendToArray(array, data);

	//set pointer of new data to specified index

	void* tmp = 0;
	for (int i = (array->Size - 1); i > index; i--) {
		tmp = array->Data[i - 1];
		array->Data[i - 1] = array->Data[i];
		array->Data[i] = tmp;
	}
}
