#pragma once

#include "Mesh.h"
#include "math/math.hpp"

#include <vector>

class Cube : public Mesh {
  std::vector<Polygon> quads;
  math::vector origin_;
  math::vector rotation_;
  math::vector scale_{1, 1, 1};
  mutable std::vector<Polygon> cached;

  Polygon *front_;

public:
  Cube(math::vector origin = {});

  math::vector &origin() override { return origin_; }
  math::vector origin() const override { return origin_; }
  math::vector &rotation() override { return rotation_; }
  math::vector rotation() const override { return rotation_; }

  math::vector &scaling() override { return scale_; }
  math::vector scaling() const override { return scale_; }
  Polygon front() const override { return *front_; }

  void draw(MeshRenderer &renderer) const override;

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

  void calc_normal() override;
};
