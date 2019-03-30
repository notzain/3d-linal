#pragma once

#include "Bullet.h"
#include "Object.h"
#include "math/math.hpp"
#include <algorithm>
#include <cmath>
#include <vector>

class Spaceship : public Object {
  Polygon *front_ = nullptr;

public:
  Spaceship(const std::string &filename) : Object(filename) {
    /**/
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

    front_ = &*std::min_element(
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

  void update(float dt) {
    const auto front = math::vector{
        cosf(math::to_radians(90) + rotation().x) * cosf(rotation().y),
        sinf(rotation().y),
        sinf(math::to_radians(90) + rotation().x) * cosf(rotation().y)};
    const auto direction = front.normalized();
  }

  Bullet shoot() {
    const auto front = math::vector{cosf(rotation().x) * cosf(rotation().y),
                                    sinf(rotation().y),
                                    sinf(rotation().x) * cosf(rotation().y)};
    const auto direction = front.normalized();

    return {origin(), rotation()};
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
