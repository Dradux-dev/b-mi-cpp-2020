#include <iostream>

#include "vec3f.h"

int main(int, char**) {
    static_assert (sizeof(my::Vec3f) == 3*sizeof(float), "Vec3f has wrong size");

	std::cout << "Hello World" << std::endl;

	return 0;
}

