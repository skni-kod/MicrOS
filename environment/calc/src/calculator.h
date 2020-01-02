#define _CRT_SECURE_NO_WARNINGS
#include "cstring.h"
#include "array.h"
#include <stdint.h>
#include <stdbool.h>

#ifndef CALCULATOR_H
#define CALCULATOR_H

	/* Calculator */

	typedef enum objectType {
		number,
		addition,
		subtraction,
		multiplication,
		division,
		power,
		modulo,
		leftBracket,
		rightBracket,
		comma,
		factorial,
		flog,
		fsin,
		fcos,
		ftg,
		fctg,
		constPi,
		constE,
		undefined
	} objectType;

	typedef char byte;

	bool isOperator(objectType operator);
	bool isFunction(objectType operator);
	bool isConst(objectType operator);
	byte getOperatorPriority(objectType operator);
	byte getOperatorTie(objectType operator);
	objectType getObjectType(char* string);
	char* getValue(objectType operator);
	char** parseString(char* string,size_t* sizeOfArray);
	char** toPostfix(char** array, size_t* sizeOfArray);
	double factorialFunction(double* value);
	double logarithmFunction(double* base, double* value);
	double** calculateValue(double** array, size_t* sizeOfArray, objectType operator);
	double calculateString(char* string);
	double calculatePostfix(char** array, size_t* sizeOfArray);

#endif
