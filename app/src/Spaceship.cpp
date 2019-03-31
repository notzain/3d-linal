#include "Spaceship.h"

#include <numeric>

Spaceship::Spaceship(const std::string &filename) : Object(filename) {
  /*
  polygons = {
      // NORTH
      Polygon{{math::vector(0.0f, 0.0f, 0.0f), math::vector(0.0f, 1.0f, 0.0f),
               math::vector(1.0f, 1.0f, 0.0f),
               math::vector(1.0f, 0.0f, 0.0f)}},
      // SOUTH
      Polygon{{math::vector(1.0f, 0.0f, 1.0f), math::vector(1.0f, 1.0f, 1.0f),
               math::vector(0.0f, 1.0f, 1.0f),
               math::vector(0.0f, 0.0f, 1.0f)}},
      // EAST
      Polygon{{math::vector(1.0f, 0.0f, 0.0f), math::vector(1.0f, 1.0f, 0.0f),
               math::vector(1.0f, 1.0f, 1.0f),
               math::vector(1.0f, 0.0f, 1.0f)}},
      // WEST
      Polygon{{math::vector(0.0f, 0.0f, 1.0f), math::vector(0.0f, 1.0f, 1.0f),
               math::vector(0.0f, 1.0f, 0.0f),
               math::vector(0.0f, 0.0f, 0.0f)}},
      // TOP
      Polygon{{math::vector(0.0f, 1.0f, 0.0f), math::vector(0.0f, 1.0f, 1.0f),
               math::vector(1.0f, 1.0f, 1.0f),
               math::vector(1.0f, 1.0f, 0.0f)}},
      // BOTTOM
      Polygon{{math::vector(1.0f, 0.0f, 1.0f), math::vector(0.0f, 0.0f, 1.0f),
               math::vector(0.0f, 0.0f, 0.0f),
               math::vector(1.0f, 0.0f, 0.0f)}}};

  for (auto &polygon : polygons) {
    for (auto &vertex : polygon.vertices) {
      vertex += math::vector(-.5, -.5, 0);
    }
  }
  cached = polygons;
      */

  front_ = &*std::min_element(
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
