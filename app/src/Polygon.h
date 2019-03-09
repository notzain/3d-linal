#pragma once

#include "math/vector.hpp"
#include <array>

template <size_t N> struct Polygon {
  static constexpr size_t size = N;
  std::array<math::vector, N> vertices{};
  math::vector normal{};
};

using Triangle = Polygon<3>;
using Quad = Polygon<4>;