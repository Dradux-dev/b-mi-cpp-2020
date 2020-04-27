#pragma once

#include "vec3f.h"

namespace ti2 {
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
          : a(a),
            b(b)
      {}

      operator float();

      const Vec3f& a;
      const Vec3f& b;
    };
}
