#pragma once

#include "math/vector.hpp"
#include <vector>

namespace {

// http://www.iquilezles.org/www/articles/areas/areas.htm
static math::vector normal_of(bool clockwise, const math::vector &a,
                              const math::vector &b, const math::vector &c) {

  return (b - a).cross_product(c - a) * (clockwise ? 1 : -1);
}
static math::vector normal_of(bool clockwise, const math::vector &a,
                              const math::vector &b, const math::vector &c,
                              const math::vector &d) {
  return normal_of(clockwise, a, b, d) + normal_of(clockwise, b, c, d);
}
} // namespace

struct Polygon {
  std::vector<math::vector> vertices{};
  math::vector distance;
  math::vector normal{};

  void calculate_normal(bool clockwise = true) {
    distance = vertices[0];

    // Quad
    if (vertices.size() == 4) {
      normal = normal_of(clockwise, vertices[0], vertices[1], vertices[2],
                         vertices[3])
                   .normalized();
    }
    // Triangle
    else if (vertices.size() == 3) {
      normal = normal_of(clockwise, vertices[0], vertices[1], vertices[2])
                   .normalized();
    }
  }
};
