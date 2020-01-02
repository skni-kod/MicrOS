#define _CRT_SECURE_NO_WARNINGS
#include "../cstring.h"
#include <stdlib.h>
#include <string.h>

/* Make string empty */
char* emptyString(char* string) {
	//If string is null 
	if (string == NULL || string == 0) {
		return 0;
	}
	
	free(string);

	return 0;
}
