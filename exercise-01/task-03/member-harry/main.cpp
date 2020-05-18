#include "vec.h"
#include <iostream>
#include <assert.h>
#include <vector>
#include <random>

using namespace my;
using vec3f = Vec<float,3>;

template <typename T, int N>
void printLen(const std::vector<Vec<T,N>>& vectors) {
	for(const Vec<T,N>& vec : vectors) {
		std::cout << "|" << vec << "|" << " = " << vec.length() << std::endl;
		}
}

int main(int , char** ) {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> dis(0.0f,100.0f);

	std::cout <<"V1: " << std::endl;
	std::vector<vec3f> v1(10);
	std::generate(v1.begin(),v1.end(),[&dis,&gen](){
			return vec3f({dis(gen),dis(gen),dis(gen)});
		});
	printLen(v1);

	std::cout << "V2: " << std::endl;
	std::vector<vec3f> v2(10);
	std::transform(v1.begin(),v1.end(),v2.begin(), [](const vec3f& vec) {
			return vec +vec3f({1,1,1});
		});
	printLen(v2);

	std::cout << "V3: " << std::endl;
	std::vector<vec3f> v3(v2);
	std::stable_partition(v3.begin(), v3.end(), [](const vec3f& n){
			return n.length() < 90.0f;
		});
	printLen(v3);

	std::cout << "V4: " << std::endl;
	std::vector<vec3f> v4(v3);
	std::sort(v4.begin(), v4.end(), [](const vec3f& a, const vec3f& b){
			return a.length() < b.length();
		});
	printLen(v4);

	std::cout <<"Median: " << v4[4] << std::endl;

	std::cout << "V5: " << std::endl;
	std::vector<vec3f> v5;
	std::copy_if(v4.begin(), v4.end(), std::back_inserter(v5), [](const vec3f& a) -> bool {
			return a.length() < 80.0f;
		});
	printLen(v5);

	std::cout << "V6: " << std::endl;
	std::vector<vec3f> v6(v1);
	v6.erase(std::remove_if(v6.begin(), v6.end(), [](const vec3f& a) -> bool {
			float length = a.length();
			return  length > 90.0f && length < 120.0f;
		}),v6.end());
	printLen(v6);

	return 0;
}


