#pragma once

#include "Bullet.h"
#include "Object.h"
#include "math/math.hpp"
#include <algorithm>
#include <cmath>
#include <vector>

class Spaceship : public Object {
  Polygon *front_ = nullptr;
  Bullet bullet_;
  float velocity = 1;

public:
  Spaceship(const std::string &filename) : Object(filename), bullet_({}, {}) {
    float total_x = 0;
    float total_y = 0;
    int total_v = 0;

    for (const auto &p : cached) {
      for (const auto &v : p.vertices) {
        total_x += v.x;
        total_y += v.y;
        total_v++;
      }
    }

    float avg_x = total_x / total_v;
    float avg_y = total_y / total_v;

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

    bullet_.color[1] = 0;
    bullet_.color[2] = 0;
  }
  Polygon front() const override { return *front_; }

  void update(float dt) {
    const auto front = math::vector{
        cosf(math::to_radians(90) + rotation().x) * cosf(rotation().y),
        sinf(rotation().y),
        sinf(math::to_radians(90) + rotation().x) * cosf(rotation().y)};
    const auto direction = front.normalized();
    origin_ += (direction * velocity * dt);
  }

  void shoot() {
    const auto front = math::vector{cosf(rotation().x) * cosf(rotation().y),
                                    sinf(rotation().y),
                                    sinf(rotation().x) * cosf(rotation().y)};
    const auto direction = front.normalized();
  }

  void draw(MeshRenderer &renderer) const override {
    bullet_.draw(renderer);
    renderer.draw(cached,
                  sf::Color(color[0] * 255, color[1] * 255, color[2] * 255));

    cached = polygons;
  }

  void rotate(const math::matrix &matrix) override {
    bullet_.rotate(matrix);

    for (auto &polygon : cached) {
      for (auto &vertex : polygon.vertices) {
        vertex = math::multiply(vertex, matrix);
      }
    }
  }
  void scale(const math::matrix &matrix) override {
    bullet_.scale(matrix);

    for (auto &polygon : cached) {
      for (auto &vertex : polygon.vertices) {
        vertex = math::multiply(vertex, matrix);
      }
    }
  }
  void translate(const math::matrix &matrix) override {
    bullet_.translate(matrix);

    for (auto &polygon : cached) {
      for (auto &vertex : polygon.vertices) {
        vertex += math::vector(matrix(3, 0), matrix(3, 1), matrix(3, 2));
      }
    }
  }
  void project(const math::matrix &matrix) override {
    bullet_.project(matrix);

    for (auto &polygon : cached) {
      for (auto &vertex : polygon.vertices) {
        vertex = math::multiply(vertex, matrix);
        vertex = vertex / vertex.w;
      }
    }
  }
};
