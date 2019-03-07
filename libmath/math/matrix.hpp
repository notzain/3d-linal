#pragma once
#include <cstddef>

namespace math {
struct matrix {
private:
  float values[4][4] = {0};

public:
  float &operator()(size_t x, size_t y);
  float operator()(size_t x, size_t y) const;

  matrix &operator*=(const matrix& other);
  matrix operator*(const matrix& other) const;
};
} // namespace math