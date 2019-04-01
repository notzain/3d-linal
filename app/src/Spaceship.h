#pragma once

#include "Bullet.h"
#include "Object.h"
#include "math/math.hpp"
#include <vector>

class Spaceship : public Object {
  Polygon *front_ = nullptr;
  float velocity = 0;

public:
  Spaceship(const std::string &filename);
  Polygon front() const override { return *front_; }

  void look_z(float dt) { rotation_.z += dt; }
  void look_y(float dt) { rotation_.y += dt; }
  void look_x(float dt) { rotation_.x += dt; }

  void update(float dt) {
    auto direction = math::rotation_to_direction({0, 0, 1}, rotation());
    direction.normalize();

    origin_ += direction * velocity * dt;
  }

  void accelerate(float dt) {
    velocity += dt;
    velocity = std::clamp(velocity, 0.f, 5.f);
  }

  Bullet shoot() { return {origin(), rotation(), velocity}; }

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
