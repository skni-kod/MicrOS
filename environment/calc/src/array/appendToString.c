#define _CRT_SECURE_NO_WARNINGS
#define _USE_MATH_DEFINES
#include "../array.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* Append text to string */
void appendToString(Object* obj, const char* string){
	
	if (obj == NULL || obj->Type != String) {
		return;
	}

	obj->Length = sizeof(char) * (strlen(string) + strlen(obj->Data) + 1);

	/* Concatenate two strings in new variable */
	char* tmp = malloc(obj->Length);
	if (tmp != NULL) {
		strcpy(tmp, obj->Data);
		strcat(tmp, string);

		free(obj->Data);
		obj->Data = tmp;
	}
	else {
		obj->Length = sizeof(char) * (strlen(obj->Data) + 1);
		exit(-1);
	}
}
