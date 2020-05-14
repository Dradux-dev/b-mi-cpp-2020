#pragma once
#ifndef _vec_h_
#define _vec_h_
#include <array>
namespace my {
	template <typename T, int N>
	class Vec {
	private:
		std::array<T, N> arr;
	public:
		Vec();
		Vec(T arg1);
		Vec(T arg1, T arg2);
		Vec(T arg1, T arg2, T arg3);
		template<typename ...Args>
		Vec(T& first, const Args& ...rest);
		const T& operator[](const int i) const;
		T& operator[](int i);
		bool operator==(const Vec& lhs);
		bool operator!=(const Vec& lhs);
		Vec<T,N>& operator+=(const Vec& lhs);
		Vec<T,N>& operator+(const Vec& lhs);
		Vec<T,N> operator-();
		friend T dot(const Vec<T,N>& a, const Vec<T,N>& b);
	};
	template <typename T, int N >
	T dot(const Vec<T,N>& a, const Vec<T,N>& b);
	template<typename T, int N>
	T dot(const Vec<T, N>& a, const Vec<T, N>& b)
	{
		return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
	}
}
#include "vec.cpp"
#endif // !_vec_h_