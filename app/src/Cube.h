#pragma once

#include "Mesh.h"
#include "math/math.hpp"
#include <algorithm>

class Cube : public Mesh {
  std::vector<Quad> quads;
  math::vector origin_;
  math::vector rotation_;

public:
  mutable std::vector<Quad> cached;

public:
  Cube(math::vector origin = {}) {
    origin_ = origin;
    // clang-format off
    quads = {
        // SOUTH
        {
          math::vector(0.0f, 0.0f, 0.0f),
          math::vector(0.0f, 1.0f, 0.0f),
          math::vector(1.0f, 1.0f, 0.0f),
          math::vector(1.0f, 0.0f, 0.0f),
        },
        // NORTH
        {
          math::vector(1.0f, 0.0f, 1.0f),
          math::vector(1.0f, 1.0f, 1.0f),
          math::vector(0.0f, 1.0f, 1.0f),
          math::vector(0.0f, 0.0f, 1.0f),
        },
        // EAST
        {
          math::vector(1.0f, 0.0f, 0.0f),
          math::vector(1.0f, 1.0f, 0.0f),
          math::vector(1.0f, 1.0f, 1.0f),
          math::vector(1.0f, 0.0f, 1.0f),
        },

        // WEST
        {
          math::vector(0.0f, 0.0f, 1.0f),
          math::vector(0.0f, 1.0f, 1.0f),
          math::vector(0.0f, 1.0f, 0.0f),
          math::vector(0.0f, 0.0f, 0.0f),
        },
        // TOP
        {
          math::vector(0.0f, 1.0f, 0.0f),
          math::vector(0.0f, 1.0f, 1.0f),
          math::vector(1.0f, 1.0f, 1.0f),
          math::vector(1.0f, 1.0f, 0.0f),
        },
        // BOTTOM
        {
          math::vector(1.0f, 0.0f, 1.0f),
          math::vector(0.0f, 0.0f, 1.0f),
          math::vector(0.0f, 0.0f, 0.0f),
          math::vector(1.0f, 0.0f, 0.0f),
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
  }

  math::vector &origin() override { return origin_; }
  math::vector origin() const override { return origin_; }
  math::vector &rotation() override { return rotation_; }
  math::vector rotation() const override { return rotation_; }

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
      math::vector line_a = polygon.vertices[1] - polygon.vertices[0];
      math::vector line_b = polygon.vertices[3] - polygon.vertices[0];

      polygon.normal = line_a.normalized().cross_product(line_b.normalized());
    }
  }
};
