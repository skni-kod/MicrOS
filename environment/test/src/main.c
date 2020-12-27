#include <stdio.h>
#include <collections.h>

int main(int argc, char *argv[])
{
	Object *tmp = ObjectInit("HelloWorld!");
	ObjectPrint(tmp);
	ObjectSet(tmp, "No, elo.");
	ObjectPrint(tmp);
	return 0;
}