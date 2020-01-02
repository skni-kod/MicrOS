#define _CRT_SECURE_NO_WARNINGS
#include "../array.h"
#include "../cstring.h"
#include <stdio.h>
#include <string.h>

/* Edit value in array */
void** editInArray(void** array, void* value, size_t* sizeOfArray, size_t index, Type type) {
	/*
	//If array is empty
	if (array == NULL || *sizeOfArray < 1) {
		//If sizeOfArray is wrong arg do not make memory leak of array
		if (array != NULL) {
			return array;
		}
		return 0;
	}

	//Given index is wrong
	if (index > *sizeOfArray) {
		return array;
	}

	if(array[index] != NULL)
		switch (type) {
		case tString:
			free((char*)array[index]);
			break;
		case tInt:
			free((int*)array[index]);
			break;
		case tDouble:
			free((double*)array[index]);
			break;
	}

	//Allocate memory for value
	switch (type) {
	case tString:
		array[index] = (char*)malloc((strlen((char*)value) + 1) * sizeof(char));
		break;
	case tInt:
		array[index] = (int*)malloc(sizeof(int));
		break;
	case tDouble:
		array[index] = (double*)malloc(sizeof(double));
		break;
	};

	return array;
	*/
}