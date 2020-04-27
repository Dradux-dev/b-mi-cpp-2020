#pragma once

#include <array>

namespace my {
    class Vec3f
    {
        public:
            static float Precision;

            inline Vec3f ()
                : Vec3f(0.0f, 0.0f, 0.0f)
            {}

            inline Vec3f(float x, float y, float z)
                : components({x, y, z})
            {}

            Vec3f(const Vec3f&) = default;

            inline float& operator[](size_t pos) {
                return components[pos];
            }

            inline const float& operator[](size_t pos) const {
                return components[pos];
            }

            bool operator==(const Vec3f& other) const;

            inline bool operator!=(const Vec3f& other) const {
                return !(*this == other);
            }

            Vec3f& operator +=(const Vec3f& other);

            inline Vec3f operator+(const Vec3f& other) {
                return (Vec3f(*this) += other);
            }

            inline Vec3f operator-() {
                return Vec3f(
                    -(*this)[0],
                    -(*this)[1],
                    -(*this)[2]
                );
            }

        protected:
            std::array<float, 3> components;
    };
}
