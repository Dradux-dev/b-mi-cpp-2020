#pragma once

#include <array>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <ostream>
#include <iterator>

namespace my {
	template <typename T, int N>
	class Vec {
	private:
		std::array<T, N> components;
	public:
		inline Vec(){
			static_assert (N>=1);
			static_assert(sizeof(*this) == N*sizeof(T));
			std::fill(components.begin(),components.end(),T());
		}
		Vec(const Vec& other)  = default;

		inline Vec(const std::initializer_list<T>& lst) : Vec() {

			std::copy_n(lst.begin(), std::min(N, static_cast<int>(lst.size())), components.begin());
		}

		inline const T& operator[](const size_t i) const {
			return components[i];
		}
		inline T& operator[](const size_t i) {
			return components[i];
		}

		inline bool operator==(const Vec& lhs) const{
			return components == lhs.components;
		}
		inline bool operator!=(const Vec& lhs) const{
			return !(*this == lhs);
		}

		Vec<T,N>& operator+=(const Vec& lhs) {
			 std::transform(lhs.components.begin(),lhs.components.end(),components.begin(),components.begin(), std::plus<T>() );
			 return *this;
		}

		Vec<T,N> operator+(const Vec& lhs) const{
				return (Vec<T,N>(*this) += lhs);
		}
		Vec<T,N> operator-() const {
			Vec<T,N> result;
			std::transform(components.begin(),components.end(),result.components.begin(),std::negate<T>());
			return result;
		}

		float length() const {
			Vec<T,N> square;
			std::transform(components.begin(),components.end(),components.begin(),square.components.begin(), std::multiplies<T>());

			T sum = std::accumulate(square.components.begin(), square.components.end(),T());

			return std::sqrt(static_cast<float>(sum));
		}

#ifndef NDEBUG
		inline typename std::array<T,N>::const_iterator begin() const {
			return components.begin();
		}
		inline typename std::array<T,N>::const_iterator end() const {
			return components.end();
		}
#endif

		template <typename U, int K >
		friend U dot(const Vec<U,K>& a, const Vec<U,K>& b);

		template <typename U, int K>
		friend std::ostream& operator<<(std::ostream& os, const Vec<U,K>& v);
	};

	template <typename U, int K >
	U dot(const Vec<U,K>& a, const Vec<U,K>& b)
	{
		return std::inner_product(a.components.begin(), a.components.end(), b.components.begin(), U());
	}

	template <typename U, int K>
	std::ostream& operator<<(std::ostream& os, const Vec<U,K>& v) {
		os << "[";
		std::copy(v.components.begin(),v.components.end()-1,std::ostream_iterator<U>(os, ", "));
		os << v.components[K-1];
		os << "]";

		return os;
	}
}

