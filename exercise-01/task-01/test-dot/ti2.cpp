#include "ti2.h"

namespace ti2 {
    dot::operator float() {
        return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
    }
}
