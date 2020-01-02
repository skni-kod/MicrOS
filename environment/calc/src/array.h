#define _CRT_SECURE_NO_WARNINGS
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#ifndef ARRAY_H
#define ARRAY_H

	/* Array  */

	typedef enum Type {
		tString,
		tInt,
		tDouble,
	} Type;

	void freeArray(void** array, size_t* sizeOfArray, Type type);
	void printArray(void** array, size_t* sizeOfArray, Type type);
	void** popArray(void** array, size_t* sizeOfArray, bool deleteValue, Type type);
	void** removeInArray(void** array, size_t* sizeOfArray, size_t index,bool deleteValue, Type type);
	void** appendToArray(void** array, const void* value, size_t* sizeOfArray, Type type);
	void** insertToArray(void** array, void* value, size_t* sizeOfArray, size_t index, Type type);
	void** editInArray(void** array, void* value, size_t* sizeOfArray, size_t index, Type type);

#endif