#define _CRT_SECURE_NO_WARNINGS
#define _USE_MATH_DEFINES
#include "array.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int main(int argc, char** argv)
{
	Array* main = initArray();
	Object* obj = initObject("69.34", String);
	convertToInt(obj);
	printObject(obj);

	return 0;
}
