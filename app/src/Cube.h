#pragma once

#include "Mesh.h"
#include "math/math.hpp"
#include <algorithm>

#include <vector>

class Cube : public Mesh {
  std::vector<Polygon> quads;
  math::vector origin_;
  math::vector rotation_;
  math::vector scale_{1, 1, 1};
  mutable std::vector<Polygon> cached;

  Polygon *front_;

public:
  Cube(math::vector origin = {}) {
    origin_ = origin;
    // clang-format off
    quads = {
      //NORTH
      Polygon {
        {
          math::vector(0.0f, 0.0f, 0.0f),
          math::vector(0.0f, 1.0f, 0.0f),
          math::vector(1.0f, 1.0f, 0.0f),
          math::vector(1.0f, 0.0f, 0.0f)
        }
      },
      //SOUTH
      Polygon {
        {
          math::vector(1.0f, 0.0f, 1.0f),
          math::vector(1.0f, 1.0f, 1.0f),
          math::vector(0.0f, 1.0f, 1.0f),
          math::vector(0.0f, 0.0f, 1.0f)
        }
      },
      //EAST
      Polygon {
        {
          math::vector(1.0f, 0.0f, 0.0f),
          math::vector(1.0f, 1.0f, 0.0f),
          math::vector(1.0f, 1.0f, 1.0f),
          math::vector(1.0f, 0.0f, 1.0f)
        }
      },
      //WEST
      Polygon {
        {
          math::vector(0.0f, 0.0f, 1.0f),
          math::vector(0.0f, 1.0f, 1.0f),
          math::vector(0.0f, 1.0f, 0.0f),
          math::vector(0.0f, 0.0f, 0.0f)
        }
      },
      //TOP
      Polygon {
        {
          math::vector(0.0f, 1.0f, 0.0f),
          math::vector(0.0f, 1.0f, 1.0f),
          math::vector(1.0f, 1.0f, 1.0f),
          math::vector(1.0f, 1.0f, 0.0f)
        }
      },
      //BOTTOM
      Polygon {
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
          float z_a =
              std::accumulate(a.vertices.begin(), a.vertices.end(), 0.f,
                              [](float begin, const math::vector &vert) {
                                return begin + vert.z;
                              });
          float z_b =
              std::accumulate(b.vertices.begin(), b.vertices.end(), 0.f,
                              [](float begin, const math::vector &vert) {
                                return begin + vert.z;
                              });

          return z_a > z_b;
        });
  }

  math::vector &origin() override { return origin_; }
  math::vector origin() const override { return origin_; }
  math::vector &rotation() override { return rotation_; }
  math::vector rotation() const override { return rotation_; }

  math::vector &scaling() override { return scale_; }
  math::vector scaling() const override { return scale_; }
  Polygon front() const override { return *front_; }

  void draw(MeshRenderer &renderer) const override {
    renderer.draw(cached,
                  sf::Color(color[0] * 255, color[1] * 255, color[2] * 255));

    cached = quads;
  }

  void rotate(const math::matrix &matrix) override {
    for (auto &polygon : cached) {
      for (auto &vertex : polygon.vertices) {
        vertex = math::multiply(vertex, matrix);
      }
    }
  }
  void scale(const math::matrix &matrix) override {
    for (auto &polygon : cached) {
      for (auto &vertex : polygon.vertices) {
        vertex = math::multiply(vertex, matrix);
      }
    }
  }
  void translate(const math::matrix &matrix) override {
    for (auto &polygon : cached) {
      for (auto &vertex : polygon.vertices) {
        vertex += math::vector(matrix(3, 0), matrix(3, 1), matrix(3, 2));
      }
    }
  }
  void project(const math::matrix &matrix) override {
    for (auto &polygon : cached) {
      for (auto &vertex : polygon.vertices) {
        vertex = math::multiply(vertex, matrix);
        vertex = vertex / vertex.w;
      }
    }
  }

  void calc_normal() override {
    for (auto &polygon : cached) {
      polygon.calculate_normal();
    }
  }
};
