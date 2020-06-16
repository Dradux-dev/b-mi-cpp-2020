#include "vec3f.h"
my::Vec3f::Vec3f()
{
	arr[0] = 0;
	arr[1] = 0;
	arr[2] = 0;
}

my::Vec3f::Vec3f(float arg1, float arg2, float arg3)
{
	arr[0] = arg1;
	arr[1] = arg2;
	arr[2] = arg3;
}

float& my::Vec3f::operator[](int i)
{
	return arr[i];
}

bool my::Vec3f::operator==(const Vec3f& lhs) const
{
	return (this->arr[0] == lhs.arr[0] &&
		this->arr[1] == lhs.arr[1] &&
		this->arr[2] == lhs.arr[2]);

}

bool my::Vec3f::operator!=(const Vec3f& lhs) const
{
	return !(*this == lhs);
}

my::Vec3f& my::Vec3f::operator+=(const my::Vec3f& lhs)
{
	arr[0] += lhs.arr[0];
	arr[1] += lhs.arr[1];
	arr[2] += lhs.arr[2];
	return *this;
}

my::Vec3f my::Vec3f::operator+(const Vec3f& lhs) const
{
	return (Vec3f(*this) += lhs);
}

my::Vec3f my::Vec3f::operator-() const
{
	return Vec3f(-(*this).arr[0],
			-(*this).arr[1],
			-(*this).arr[2]);
}

const float& my::Vec3f::operator[](const int i) const
{
	return arr[i];
}

float my::dot(const Vec3f& a, const Vec3f& b)
{
	return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}
