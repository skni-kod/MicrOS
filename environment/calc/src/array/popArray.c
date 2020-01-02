#define _CRT_SECURE_NO_WARNINGS
#include "../array.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* Delete last value */
void** popArray(void** array, size_t* sizeOfArray,bool deleteValue,Type type) {
	/*
	//If array is empty
	if (array == NULL || *sizeOfArray < 1){
		//If sizeOfArray is wrong arg do not make memory leak of array
		if (array != NULL) {
			return array;
		}
		return 0;
	}

	//When array has one value
	if (*sizeOfArray == 1) {
		//Check if we have to delete value
		if (deleteValue == true) {
			switch (type) {
			case tString:
				free((char*)array);
				break;
			case tInt:
				free((int*)array);
				break;
			case tDouble:
				free((double*)array);
				break;
			}
		}
		*sizeOfArray = 0;
		return 0;
	}

	//Store pointer to array, to avoid memory leak of array
	void** tmpArray = array;

	//Pointer to last element of array
	void* tmpVal = array[*sizeOfArray - 1];

	//Delete last element from memory
	if (deleteValue == true) {
		free(array[(*sizeOfArray - 1)]);
	}

	//Allocate new space for array
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

	
	//When allocation fails, free all memory
	if(array == NULL){
		freeArray(tmpArray, *sizeOfArray,type);
		return 0;
	}

	*sizeOfArray -= 1;

	return array;
	*/
}
