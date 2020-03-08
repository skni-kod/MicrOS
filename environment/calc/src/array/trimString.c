#define _CRT_SECURE_NO_WARNINGS
#define _USE_MATH_DEFINES
#include "../array.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/*  */
void trimString(Object* object, bool direction, const char data) {
	
	if (object != NULL && object->Data != NULL && object->Type == String && object->Length > 1) {
		if (direction == true)
		{
			for (int i = (object->Length - 2); i-- >= 0;) {
				if (((char*)object->Data)[i] == data) {
					cutString(object, (object->Length - 2), 1);
				}
				else {
					cutString(object, (object->Length - 2), 1);
					break;
				}
			}
			return;
		}else{
			for (int i = 0; i <= (object->Length - 2); i++) {
				if (((char*)object->Data)[i] == data) {
					cutString(object, 0, -1);
				}
				else {
					cutString(object, 0, -2);
					break;
				}
			}
			return;
		}
	}

}
