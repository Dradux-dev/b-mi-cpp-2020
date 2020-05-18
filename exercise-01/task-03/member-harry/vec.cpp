#ifndef _VEC_CPP_
#define _VEC_CPP_
#include "vec.h"



template<typename T, int N>
const T& my::Vec<T, N>::operator[](const int i) const
{
	return (*this)[i];
}

template<typename T, int N>
T& my::Vec<T,N>::Vec::operator[](int i)
{
	return components[i];
}

template<typename T, int N>
bool my::Vec<T, N>::operator==(const Vec& lhs)
{
	if (this->components[0] == this->components[0] &&
		this->components[1] == this->components[1] &&
		this->components[2] == this->components[2]) {
		return true;
	}
	return false;
}

template<typename T, int N>
bool my::Vec<T, N>::operator!=(const Vec& lhs)
{
	return false;
}

template<typename T, int N>
my::Vec<T,N>& my::Vec<T, N>::operator+=(const my::Vec<T,N>& lhs)
{
	components[0] += lhs.components[0];
	components[1] += lhs.components[1];
	components[2] += lhs.components[2];
	return *this;
}

template<typename T, int N>
my::Vec<T,N>& my::Vec<T, N>::operator+(const Vec<T,N>& lhs)
{
	// TODO: hier return-Anweisung eingeben
}

template<typename T, int N>
my::Vec<T,N> my::Vec<T,N>::operator-()
{
	Vec<T,N> v;
	for (int i = 0; i < N; ++i) {
		v[i] = -(*this)[i];
	}
	return;
}
#endif
