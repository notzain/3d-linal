#pragma once
#include <cstdint>

namespace math {
struct matrix {
private:
  float values[4][4] = {0};

public:
  float &operator()(std::size_t x, std::size_t y);
  float operator()(std::size_t x, std::size_t y) const;

  matrix &operator*=(const matrix& other);
  matrix operator*(const matrix& other) const;
};
} // namespace math