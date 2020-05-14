#include <iostream>
#include <assert.h>
#include "vec.h"

using namespace my;
int main(int argc, char* argv) {

	int a = 1;
	int b = 2;
	Vec<int, 2> f(a,b);

	std::cout << f[1];


	return 0;
}