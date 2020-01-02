#define _CRT_SECURE_NO_WARNINGS
#include "../cstring.h"
#include <stdbool.h>
#include <string.h>

/* Check if the given string is a number */
bool isNumber(char* expression) {

	size_t expLength = strlen(expression);

	if (expLength < 1) { return false; }

	for (size_t i = 0; i < expLength; i++) {
		//check all combinations of digits stored in chars
		//we have integers, and floating point numbers with dot
		//if none of these conditions are true return false
		if (isDigit(expression[i]) == 0) {
			return false;
		}
	}

	return true;

}