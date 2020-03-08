#define _CRT_SECURE_NO_WARNINGS
#define _USE_MATH_DEFINES
#include "../array.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/*  */
bool isNumber(const char* data) {

	size_t expLength = strlen(data);

	if (expLength < 1) { return false; }

	for (size_t i = 0; i < expLength; i++) {
		//check all combinations of digits stored in chars
		//we have integers, and floating point numbers with dot
		//if none of these conditions are true return false
		if (isDigit(data[i]) == 0) {
			return false;
		}
	}

	return true;

}
