#include "vec.h"
#include <iostream>
#include <assert.h>
#include "test11.h"
#include "test.h"
using namespace my;
int main(int , char** ) {

	static_assert(sizeof(Vec<float,3>) == 3*sizeof(float));
	test_Vec3f();
	test_Vec<int,2>();
	test_Vec<double,4>();

	return 0;
}
