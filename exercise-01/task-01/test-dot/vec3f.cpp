#include "vec3f.h"

#include <cmath>

namespace my {
    namespace helper {
        static inline bool equals(float a, float b) {
            return (std::abs(a - b) <= Vec3f::Precision);
        }
    }

    float Vec3f::Precision = 0.000001f;

    bool Vec3f::operator==(const Vec3f& other) const {
        using namespace helper;
        return (equals((*this)[0], other[0]) && equals((*this)[1], other[1]) && equals((*this)[2], other[2]));
    }

    Vec3f& Vec3f::operator+=(const Vec3f& other) {
        components = {
          (*this)[0] + other[0],
          (*this)[1] + other[1],
          (*this)[2] + other[2]
        };

        return *this;
    }
}
