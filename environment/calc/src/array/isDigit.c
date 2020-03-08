#define _CRT_SECURE_NO_WARNINGS
#define _USE_MATH_DEFINES
#include "../array.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/*  */
bool isDigit(const char data) {

	if ((data >= '0' && data <= '9') || data == '.') {
		return true;
	}
	return false;

}
