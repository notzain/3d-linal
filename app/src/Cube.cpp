#include "Cube.h"

#include <numeric>

Cube::Cube(math::vector origin) {
  origin_ = origin;
  // clang-format off
  quads = {
    //NORTH
    Polygon{
      {
        math::vector(0.0f, 0.0f, 0.0f),
        math::vector(0.0f, 1.0f, 0.0f),
        math::vector(1.0f, 1.0f, 0.0f),
        math::vector(1.0f, 0.0f, 0.0f)
      }
    },
    //SOUTH
    Polygon{
      {
        math::vector(1.0f, 0.0f, 1.0f),
        math::vector(1.0f, 1.0f, 1.0f),
        math::vector(0.0f, 1.0f, 1.0f),
        math::vector(0.0f, 0.0f, 1.0f)
      }
    },
    //EAST
    Polygon{
      {
        math::vector(1.0f, 0.0f, 0.0f),
        math::vector(1.0f, 1.0f, 0.0f),
        math::vector(1.0f, 1.0f, 1.0f),
        math::vector(1.0f, 0.0f, 1.0f)
      }
    },
    //WEST
    Polygon{
      {
        math::vector(0.0f, 0.0f, 1.0f),
        math::vector(0.0f, 1.0f, 1.0f),
        math::vector(0.0f, 1.0f, 0.0f),
        math::vector(0.0f, 0.0f, 0.0f)
      }
    },
    //TOP
    Polygon{
      {
        math::vector(0.0f, 1.0f, 0.0f),
        math::vector(0.0f, 1.0f, 1.0f),
        math::vector(1.0f, 1.0f, 1.0f),
        math::vector(1.0f, 1.0f, 0.0f)
      }
    },
    //BOTTOM
    Polygon{
      {
        math::vector(1.0f, 0.0f, 1.0f),
        math::vector(0.0f, 0.0f, 1.0f),
        math::vector(0.0f, 0.0f, 0.0f),
        math::vector(1.0f, 0.0f, 0.0f)
      }
    }
  };
  // clang-format on

  // cube isnt center on origin (0,0,0), editing the co-ordinates ^^^ is
  // annoying
  for (auto &polygon : quads) {
    for (auto &vertex : polygon.vertices) {
      vertex += math::vector(-.5, -.5, 0);
    }
  }
  cached = quads;

  front_ = &*std::max_element(
      cached.begin(), cached.end(), [](const Polygon &a, const Polygon &b) {
        float z_a = std::accumulate(a.vertices.begin(), a.vertices.end(), 0.f,
                                    [](float begin, const math::vector &vert) {
                                      return begin + vert.z;
                                    });
        float z_b = std::accumulate(b.vertices.begin(), b.vertices.end(), 0.f,
                                    [](float begin, const math::vector &vert) {
                                      return begin + vert.z;
                                    });

        return z_a > z_b;
      });
}

void Cube::draw(MeshRenderer &renderer) const {
  renderer.draw(cached,
                sf::Color(color[0] * 255, color[1] * 255, color[2] * 255));

  cached = quads;
}

void Cube::calc_normal() {
  for (auto &polygon : cached) {
    polygon.calculate_normal();
  }
}
