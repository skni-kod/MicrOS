#define _CRT_SECURE_NO_WARNINGS
#define _USE_MATH_DEFINES
#include "../array.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/*  */
void reverseString(Object* object) {

	if (object != NULL && object->Type == String && object->Data != NULL) {
		Object* tmp = initObject("", String);
		char tmpChar[2] = "";
		for (int i = (object->Length - 2); i >= 0; i--) {
			tmpChar[0] = ((char*)object->Data)[i];
			appendToString(tmp, tmpChar);
		}
		setObject(object, tmp->Data, String);
	}

}
