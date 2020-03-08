#define _CRT_SECURE_NO_WARNINGS
#define _USE_MATH_DEFINES
#include "../array.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/*  */
void splitString(Array* array, Object* object, const char* delimiter) {

	if (array != NULL && object->Type == String && object->Data != NULL && delimiter != NULL && strlen(delimiter) > 0) {
		Object* tmpStr = initObject("", String);
		size_t delimiterLength = strlen(delimiter);
		char tmpChar[2] = "";

		for (unsigned int i = 0; i < (object->Length - 1); i++) {
			tmpChar[0] = ((char*)object->Data)[i];
			appendToString(tmpStr, tmpChar);
			if ((tmpStr->Length - 1) >= delimiterLength) {
				bool flag = true;
				for (unsigned int x = 0; x < delimiterLength; x++) {
					if (((char*)tmpStr->Data)[((tmpStr->Length) - 2 - x)] != delimiter[(delimiterLength - x - 1)]) {
						flag = false;
					}
				}
				if (flag == true) {
					cutString(tmpStr, (tmpStr->Length - 2), delimiterLength);
					appendToArray(array, tmpStr);
					setObject(tmpStr, "", String);
				}
			}
		}
		if (tmpStr->Length > 1) {
			cutString(tmpStr, (tmpStr->Length - 2), delimiterLength);
			appendToArray(array, tmpStr);
			setObject(tmpStr, "", String);
		}
	}

}
