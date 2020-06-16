#include "vec3f.h"
#include <iostream>
#include <assert.h>
#include "test11.h"
using namespace my;
int main(int argc, char** argv) {

	static_assert(sizeof(Vec3f) == 3*sizeof(float));
	test_Vec3f();

	return 0;
}
