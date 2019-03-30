#pragma once

#include "Cube.h"
#include "Object.h"
#include "math/math.hpp"
#include <algorithm>
#include <cmath>
#include <vector>

class Spaceship : public Object {
  Polygon *front_ = nullptr;

public:
  Spaceship(const std::string &filename) : Object(filename) {
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
  Polygon front() const override { return *front_; }

  void shoot() {
    const auto front = math::vector{cosf(rotation().x) * cosf(rotation().y),
                                    sinf(rotation().y),
                                    sinf(rotation().x) * cosf(rotation().y)};
    const auto direction = front.normalized();
  }

  void draw(MeshRenderer &renderer) const override {
    renderer.draw(cached,
                  sf::Color(color[0] * 255, color[1] * 255, color[2] * 255));

    cached = polygons;
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
};
