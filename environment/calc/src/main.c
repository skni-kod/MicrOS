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
	int x = 343;
	Object* obj = initObject(&x, Int);
	convertToDouble(obj);
	printObject(obj);

	return 0;
}
