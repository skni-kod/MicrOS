#include <stdio.h>
#include <collections.h>
int main(int argc, char *argv[])
{
	Object *tmp = ObjectInit("1234567");
	ObjectPrint(tmp);
	ObjectToULong(tmp);
	ObjectPrint(tmp);
	return 0;
}