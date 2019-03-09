#pragma once

#include "math/vector.hpp"
#include <vector>

struct Polygon {
  std::vector<math::vector> vertices{};
  math::vector normal{};

  void calculate_normal() {
    // Quad
    if (vertices.size() == 4) {
      const math::vector line_a = vertices[1] - vertices[0];
      const math::vector line_b = vertices[3] - vertices[0];
      normal = line_a.cross_product(line_b).normalized();
    }
    // Triangle
    else if (vertices.size() == 3) {
      const math::vector line_a = vertices[1] - vertices[0];
      const math::vector line_b = vertices[2] - vertices[0];
      normal = line_a.cross_product(line_b).normalized();
    }
  }
};
