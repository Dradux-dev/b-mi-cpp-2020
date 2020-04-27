#pragma once

#include "vec3f.h"

namespace ti3 {
    class Vec3f : public my::Vec3f {
        public:
            Vec3f() = default;
            inline Vec3f(float x, float y, float z)
                : my::Vec3f(x, y, z)
            {}

            inline const float& operator[](size_t pos) const {
                return my::Vec3f::operator[](pos);
            }

            friend struct dot;
    };

    struct dot {
      inline dot(const Vec3f& a, const Vec3f& b)
          : value(a[0] * b[0] + a[1] * b[1] + a[2] * b[2])
      {}

      inline operator float() {
          return value;
      }

      float value;
    };
}
