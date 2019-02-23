#pragma once

#include "math/vector.hpp"
#include <array>

struct Triangle {
    std::array<math::vector, 3> vertices{};
};