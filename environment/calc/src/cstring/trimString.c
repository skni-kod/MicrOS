#define _CRT_SECURE_NO_WARNINGS
#include "../cstring.h"
#include <stdlib.h>
#include <string.h>

/* Trim a string from tailing zeros */
char* trimString(char* string, const char Char) {
	
	if (string == NULL) {
		return string;
	}

	size_t sizeOfString = strlen(string);

	if (sizeOfString < 1) {
		return string;
	}

	for (size_t i = sizeOfString - 1; i-- > 0;) {
		if (string[i] == Char) {
			string = popString(string);
		} else {
			string = popString(string);
			break;
		}
	}

	return string;

}
