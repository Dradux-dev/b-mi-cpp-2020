#include "ti1.h"

#include <cmath>

namespace ti1 {
    float dot(const Vec3f& a, const Vec3f& b) {
        return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
    }
}
