#ifndef _vec_h_
#define _vec_h_
#include <array>
namespace my {
	class Vec3f {
	private:
		std::array<float, 3> arr;
	public:
		Vec3f();
		Vec3f(float arg1, float arg2, float arg3);
		const float& operator[](const int i) const;
		float& operator[](int i);
		bool operator==(const Vec3f& lhs) const;
		bool operator!=(const Vec3f& lhs) const;

		Vec3f& operator+=(const Vec3f& lhs);
		Vec3f operator+(const Vec3f& lhs) const;
		Vec3f operator-() const;
		friend float dot(const Vec3f& a, const Vec3f& b);
	};
	float dot(const Vec3f& a, const Vec3f& b);
}


#endif // !_vec_h_
