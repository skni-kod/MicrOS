#define _CRT_SECURE_NO_WARNINGS
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#ifndef ARRAY_H
#define ARRAY_H

typedef enum Type {
	String,
	Int,
	Double,
	Pointer,
} Type;

typedef struct object {
	void* Data;
	size_t Length;
	Type Type;
} Object;

typedef struct array {
	void** Data;
	size_t Size;
} Array;

	/* Array */
	void initArray(Array* array);
	void appendToArray(Array* array, const Object* object);
	void popArray(Array* array);
	void setValue(Array* array, const Object* object, const unsigned int index);
	void insertValue(Array* array, const Object* object, const unsigned int index);
	void printArray(Array* array);
	
	/* Data object */
	size_t getDataSize(const void* data, Type type);
	Object* initObject(const void* data, const Type type);
	void setData(Object* object, const void* data, const Type type);
	void freeObject(Object* object);

	void appendToString(Object* object, const char* string);


#endif