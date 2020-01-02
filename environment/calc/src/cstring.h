#define _CRT_SECURE_NO_WARNINGS
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>

#ifndef CSTRING_H
#define CSTRING_H

	/* String */
	
	bool isDigit(char string);
	bool isNumber(char* string);
	void reverseString(char* string);
	char* appendToString(char* string, char* value);
	char* emptyString(char* string);
	char* trimString(char* string,const char Char);
	char* intToString(int value);
	char* popString(char* string);
	char* doubleToString(double value);
	char** splitString(char* string, const char delimiter, size_t* sizeOfArray);


#endif