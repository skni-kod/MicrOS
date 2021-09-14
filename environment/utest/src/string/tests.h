#include <string.h>

#define ASSERTION_ERROR 1
#define ASSERTION_SUCCESS 0

int test_memcpy(){

	char src[11] = "Some text\0";
	char dst[11];

	memcpy(dst, src, 11);

	for(int i=0; i<11; i++){
		if (src[i] != dst[i]) return ASSERTION_ERROR;
	}
	

	return ASSERTION_SUCCESS;
}