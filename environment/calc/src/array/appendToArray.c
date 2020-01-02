#define _CRT_SECURE_NO_WARNINGS
#include "../array.h"
#include "../cstring.h"
#include <stdlib.h>
#include <string.h>

/* Append value to array */
void** appendToArray(void** array, const void* value, size_t* sizeOfArray, Type type) {

	//There is nothing to do 
	//!! Maybe throw error code? !!!
	if (value == NULL) {
		return array;
	}
	if((type == tString) && (strlen((char*)value) < 1)){
		return array;
	}

	//Initialize new array
	if(array == 0){
		array = malloc(sizeof(void*));		
		*sizeOfArray += 1;
		return array;
	}else{
		void** tmp = array;
		array = realloc(array,sizeof(void*) * ((*sizeOfArray)+1));
		//Allocation fails
		if(array == NULL){
			array = tmp;
			return array;
		}
	}


	//Allocate memory for value
	switch (type) {
	case tString:
		array[*sizeOfArray] = malloc((strlen((char*)value) + 1) * sizeof(char));
		break;
	case tInt:
		array[*sizeOfArray] = malloc(sizeof(int));
		break;
	case tDouble:
		array[*sizeOfArray] = malloc(sizeof(double));
		break;
	};

	//Unable to allocate memory
	if (array[*sizeOfArray] == NULL) {
		freeArray(array, sizeOfArray, type);
		return NULL;
	}

	//Copy value to array
	switch (type) {
	case tString:
		strcpy(array[*sizeOfArray], value);
		break;
	case tInt:
		memcpy(array[*sizeOfArray], value, sizeof(int));
		break;
	case tDouble:
		memcpy(array[*sizeOfArray], value, sizeof(double));
		break;
	};

	*sizeOfArray += 1;

	return array;
}