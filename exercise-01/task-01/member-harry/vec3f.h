#ifndef _vec_h_
#define _vec_h_
#include <array>
namespace my {
	class Vec3 {
	private:
		std::array<float, 3> arr;
	public:
		Vec3();
		Vec3(float arg1, float arg2, float arg3);
		const float& operator[](const int i) const;
		float operator[](int i);
		bool operator==(const Vec3& lhs);
		bool operator!=(const Vec3& lhs);
		Vec3& operator+=(const Vec3& lhs);
		Vec3& operator+(const Vec3& lhs);
		Vec3 operator-();
		friend float dot(const Vec3& a, const Vec3& b);
	};
	float dot(const Vec3& a, const Vec3& b);
}


#endif // !_vec_h_
