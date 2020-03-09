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
	Array* initArray(void);
	void appendToArray(Array* array, const Object* object);
	void popArray(Array* array);
	void deleteData(Array* array, const unsigned int index);
	void setData(Array* array, const Object* object, const unsigned int index);
	void insertValue(Array* array, const Object* object, const unsigned int index);
	void printArray(Array* array);
	
	/* Data object */
	size_t getDataSize(const void* data, Type type);
	Object* initObject(const void* data, const Type type);
	void setObject(Object* object, const void* data, const Type type);
	void printObject(Object* object);
	void freeObject(Object* object);

	/* String */
	void appendToString(Object* object, const char* string);
	void cutString(Object* object, const unsigned int index, const int count);
	void splitString(Array* array, Object* object, const char* delimiter);
	void trimString(Object* object, bool direction, const char data);
	void reverseString(Object* object);
	void convertToString(Object* object);
	void convertToInt(Object* object);
	void convertToDouble(Object* object);
	bool isDigit(const char data);
	bool isNumber(const char* data);

#endif