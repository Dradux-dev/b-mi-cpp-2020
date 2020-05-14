#ifndef _VEC_CPP_
#define _VEC_CPP_
#include "vec.h"

template<typename T, int N>
template<typename...Args>
my::Vec<T,N>::Vec(T& first, const Args& ... rest)
	: arr({first, rest...})
{
	static_assert(N == 1 + sizeof...(Args), "Vec Error: Wrong ammount of arguments!");	
}


template<typename T, int N>
my::Vec<T, N>::Vec()
{
}

template<typename T, int N>
my::Vec<T, N>::Vec(T arg1)
{
}

template<typename T, int N>
my::Vec<T, N>::Vec(T arg1, T arg2)
{
}

template<typename T, int N>
my::Vec<T, N>::Vec(T arg1, T arg2, T arg3)
{
}

template<typename T, int N>
const T& my::Vec<T, N>::operator[](const int i) const
{
	return (*this)[i];
}

template<typename T, int N>
T& my::Vec<T,N>::Vec::operator[](int i)
{
	return arr[i];
}

template<typename T, int N>
bool my::Vec<T, N>::operator==(const Vec& lhs)
{
	if (this->arr[0] == this->arr[0] &&
		this->arr[1] == this->arr[1] &&
		this->arr[2] == this->arr[2]) {
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
	arr[0] += lhs.arr[0];
	arr[1] += lhs.arr[1];
	arr[2] += lhs.arr[2];
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