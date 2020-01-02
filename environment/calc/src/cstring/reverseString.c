#define _CRT_SECURE_NO_WARNINGS
#include "../cstring.h"
#include <stdlib.h>
#include <string.h>

/* Reverse string */
void reverseString(char* string)
{
	if (string == NULL) {
		return NULL;
	}

	//Size of string
	size_t sizeOfString = strlen(string);

	if (sizeOfString < 2) {
		return NULL;
	}

	//Temporary string 
	char* tmp = 0;

	tmp = appendToString(tmp, string);

	/*
		size: 7
	   0  1  2  3  4  5  6
	   a  b  c  d  e  f  g
	  /\ index = 0

	   0  1  2  3  4  5  6
	   g  f  e  d  c  b  a

	*/

	for (size_t i = 0; i < sizeOfString; i++) {
		string[(sizeOfString-1) - i] = tmp[i];
	}

	free(tmp);

}