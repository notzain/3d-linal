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
  float velocity = 5;

  Bullet(math::vector start, math::vector direction) {

    origin_ = start;
    rotation_ = direction;

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
    // clang-format on

    for (auto &polygon : polygons) {
      for (auto &vertex : polygon.vertices) {
        vertex += math::vector(-.5, -.5, 0);
      }
    }
    cached = polygons;
  }

  void update(float dt) {
    auto direction =
        math::multiply(direction_, math::make_rotation_x(rotation_.x));
    direction = math::multiply(direction, math::make_rotation_y(rotation_.y));
    direction = math::multiply(direction, math::make_rotation_z(rotation_.z));
    direction.normalize();
    origin_ += direction * velocity * dt;
  }

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