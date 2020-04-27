#pragma once

#include <random>

#include "compare-runtime.h"
#include "vec3f.h"

namespace ti1 {
    class Vec3f : public my::Vec3f {
        public:
            Vec3f() = default;
            inline Vec3f(float x, float y, float z)
                : my::Vec3f(x, y, z)
            {}

            inline const float& operator[](size_t pos) const {
                return my::Vec3f::operator[](pos);
            }

            friend float dot(const Vec3f& a, const Vec3f& b);
    };

    float dot(const Vec3f& a, const Vec3f& b);
}
