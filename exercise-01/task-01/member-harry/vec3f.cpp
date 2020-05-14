#include "vec3f.h"
my::Vec3::Vec3()
{
	arr[0] = 0;
	arr[1] = 0;
	arr[2] = 0;
}

my::Vec3::Vec3(float arg1, float arg2, float arg3)
{
	arr[0] = arg1;
	arr[1] = arg2;
	arr[2] = arg3;
}

float my::Vec3::operator[](int i)
{
	return arr[i];
}

bool my::Vec3::operator==(const Vec3& lhs)
{
	if (this->arr[0] == this->arr[0] &&
		this->arr[1] == this->arr[1] &&
		this->arr[2] == this->arr[2]) {
		return true;
	}
	return false;
}

bool my::Vec3::operator!=(const Vec3& lhs)
{
	return !(*this == lhs);
}

my::Vec3& my::Vec3::operator+=(const my::Vec3& lhs)
{
	arr[0] += lhs.arr[0];
	arr[1] += lhs.arr[1];
	arr[2] += lhs.arr[2];
	return *this;
}

my::Vec3& my::Vec3::operator+(const Vec3& lhs)
{
	return (Vec3(*this) += lhs);
}

my::Vec3 my::Vec3::operator-()
{
	return Vec3(-(*this).arr[0],
			-(*this).arr[1],
			-(*this).arr[2]);
}

const float& my::Vec3::operator[](const int i) const
{
	return arr[i];
}

float my::dot(const Vec3& a, const Vec3& b)
{
	return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}
