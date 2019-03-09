#pragma once

#include <SFML/Graphics.hpp>
#include <algorithm>
#include <array>
#include <vector>

#include "Polygon.h"
#include "MeshRenderer.h"
#include "math/matrix.hpp"

enum class Dimension { X, Y, Z };

class Mesh {
public:
  float color[3] = {1, 1, 1};

  virtual void draw(MeshRenderer& renderer) const = 0;

  virtual void rotate(const math::matrix &matrix) = 0;
  virtual void scale(const math::matrix &matrix) = 0;
  virtual void translate(const math::matrix &matrix) = 0;
  virtual void project(const math::matrix &matrix) = 0;

  virtual math::vector origin() const = 0;
  virtual math::vector &origin() = 0;

  virtual math::vector rotation() const = 0;
  virtual math::vector &rotation() = 0;

  virtual void calc_normal() {};
};