#pragma once

#include "Mesh.h"
#include "math/math.hpp"

class Bullet : public Mesh {
  math::vector origin_;
  math::vector rotation_;
  math::vector scale_{0, 0, 1};
  math::vector direction_{0, 0, 1};
  std::vector<Polygon> polygons;
  mutable std::vector<Polygon> cached;

public:
  float velocity = 7;

  Bullet(math::vector start, math::vector direction, float velocity);

  void update(float dt);

  void draw(MeshRenderer &renderer) const override {
    renderer.draw(cached,
                  sf::Color(color[0] * 255, color[1] * 255, color[2] * 255));

    cached = polygons;
  }

  math::vector &origin() override { return origin_; }
  math::vector origin() const override { return origin_; }

  math::vector &rotation() override { return rotation_; }
  math::vector rotation() const override { return rotation_; }

  math::vector &scaling() override { return scale_; }
  math::vector scaling() const override { return scale_; }

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