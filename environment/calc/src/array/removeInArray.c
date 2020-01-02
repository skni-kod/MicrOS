#define _CRT_SECURE_NO_WARNINGS
#include "../array.h"
#include "../cstring.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Delete value at index */
void** removeInArray(void** array, size_t* sizeOfArray, size_t index, bool deleteValue, Type type) {
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

	//When index points to the last value of array,
	//just pop last value
	if (index + 1 == *sizeOfArray) {
		array = popArray(array, sizeOfArray,deleteValue, type);
		return array;
	}

	//Temporary pointer to array
	void** tmpArray = array;

	switch (type) {
	case tString:
		array = (char**)realloc(array, (*sizeOfArray - 1) * sizeof(char*));
		break;
	case tInt:
		array = (int**)realloc(array, (*sizeOfArray - 1) * sizeof(int*));
		break;
	case tDouble:
		array = (double**)realloc(array, (*sizeOfArray - 1) * sizeof(double*));
		break;
	};

	//Unable to allocate memory
	if (array == NULL) {
		freeArray(tmpArray, *sizeOfArray, type);
		return NULL;
	}

	if (deleteValue == true) {
		switch (type) {
		case tString:
			free((char*)tmpArray[index]);
			break;
		case tInt:
			free((int*)tmpArray[index]);
			break;
		case tDouble:
			free((double*)tmpArray[index]);
			break;
		}		
	}

	/*
	   0   1   2   3   4   5
	   A   B   C   D   E   F
			       /\ - index at 3
	   0   1   2   3   4
	   A   B   C   E   F


	//The last value from old array goes to last element of new array
	array[*sizeOfArray - 2] = tmpArray[*sizeOfArray - 1];

	for (size_t i = (*sizeOfArray - 3); i > index; i--) {
		array[i] = tmpArray[(i+1)];
	}

	*sizeOfArray -= 1;

	return array;
	*/
}