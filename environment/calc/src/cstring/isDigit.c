#define _CRT_SECURE_NO_WARNINGS
#include "../cstring.h"
#include <stdbool.h>

/* Check if given character is a part of number */
bool isDigit(char string) {

	if (((int)string >= 48 && (int)string <= 57) || (int)string == 46) {
		return true;
	}
	return false;

}