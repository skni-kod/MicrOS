#include "vector.h"

int main(int argc, char **argv)
{
	Vector *vec = VectorInit();
	Object *tmp = ObjectInit("HelloWord!", String);

	StringAppend(tmp, " and happy new Year!");
	VectorAppend(vec, tmp, true);
	VectorAppend(vec, ObjectInit("Hello my Friend!", String), false);
	VectorPrint(vec);

	VectorDestroy(vec);
	ObjectDelete(tmp);
	return 0;
}
