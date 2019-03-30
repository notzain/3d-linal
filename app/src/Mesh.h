#pragma once

#include <SFML/Graphics.hpp>
#include <algorithm>
#include <array>
#include <cmath>
#include <vector>

#include "MeshRenderer.h"
#include "Polygon.h"
#include "math/matrix.hpp"

enum class Dimension { X, Y, Z };

class Mesh {
public:
  float color[3] = {1, 1, 1};

  virtual void draw(MeshRenderer &renderer) const = 0;

  virtual void rotate(const math::matrix &matrix) = 0;
  virtual void scale(const math::matrix &matrix) = 0;
  virtual void translate(const math::matrix &matrix) = 0;
  virtual void project(const math::matrix &matrix) = 0;

  virtual math::vector origin() const = 0;
  virtual math::vector &origin() = 0;

  virtual math::vector rotation() const = 0;
  virtual math::vector &rotation() = 0;

  virtual math::vector &scaling() = 0;
  virtual math::vector scaling() const = 0;

  virtual Polygon front() const { return {}; }

  virtual void calc_normal() = 0;

  virtual bool checkAABB(const Mesh &other) {
#define AXIS_ALIGNS(axis)                                                      \
  (fabs(origin().axis - other.origin().axis) <                                 \
   scaling().axis + other.scaling().axis)

    if (AXIS_ALIGNS(x)) {
      if (AXIS_ALIGNS(y)) {
        if (AXIS_ALIGNS(z)) {
          return true;
        }
      }
    }

#undef AXIS_ALIGNS
    return false;
  }
};
