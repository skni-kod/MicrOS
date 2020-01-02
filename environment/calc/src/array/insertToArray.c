#define _CRT_SECURE_NO_WARNINGS
#include "../array.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Insert new value to array */
void** insertToArray(void** array, void* value, size_t* sizeOfArray, size_t index, Type type) {
	
	/*
	When array is empty append value
	if (array == NULL && *sizeOfArray == 0) {
		array = appendToArray(array, value, sizeOfArray,type);
		return array;
	}

	//Check length of array
	if (*sizeOfArray == 0 && array != NULL) {
		return array;
	}

	//When index is greater than size of array
	if (index > *sizeOfArray) {
		return array;
	}
	
	//If index points to last element of array 
	if (index == *sizeOfArray) {
		array = appendToArray(array, value, sizeOfArray,type);
		return array;
	}
	
	//Pointer to value
	void* tmp = 0;
	//Allocate memory for value
	switch (type) {
	case tString:
		tmp = (char*)malloc((strlen((char*)value) + 1) * sizeof(char));
		break;
	case tInt:
		tmp = (int*)malloc(sizeof(int));
		break;
	case tDouble:
		tmp = (double*)malloc(sizeof(double));
		break;
	};

	//Unable to allocate memory
	if (tmp == NULL) {
		freeArray(array, sizeOfArray,type);
		return NULL;
	}

	//Copy value
	switch (type) {
	case tString:
		strcpy(tmp, value);
		break;
	case tInt:
		memcpy(tmp, value, sizeof(int));
		break;
	case tDouble:
		memcpy(tmp, value, sizeof(double));
		break;
	};


	//Temporary pointer to array
	void** tmpArray = array;

	//Append value to existing array
	switch (type) {
	case tString:
		array = (char**)realloc(array, (*sizeOfArray + 1) * sizeof(char*));
		break;
	case tInt:
		array = (int**)realloc(array, (*sizeOfArray + 1) * sizeof(int*));
		break;
	case tDouble:
		array = (double**)realloc(array, (*sizeOfArray + 1) * sizeof(double*));
		break;
	};

	//Unable to allocate memory
	if (array == NULL) {
		freeArray(tmpArray, *sizeOfArray,type);
		return NULL;
	}
	
	//Move right side of array from index
	/*
	   0   1   2   3   4   5
	   A   B   C   D   E   Empty
				  /\ - index at 3
	   0   1   2   3   4   5
	   A   B   C Empty D   E   

	//Empty space will be filled with new value

	for (size_t i = (*sizeOfArray); i > index; i--) {
		array[i] = array[(i - 1)];
	}

	//Fill empty space with new value
	array[index] = tmp;
	
	*sizeOfArray += 1;

	return array;
	*/
}
