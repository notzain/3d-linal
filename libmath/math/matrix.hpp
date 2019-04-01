#pragma once
#include <cstddef>

namespace math {
class matrix {
private:
  float values[4][4] = {0};

public:
  bool operator==(const matrix &other) const {
    for (int i = 0; i < 4; ++i) {
      for (int j = 0; j < 4; ++j) {
        if (values[i][j] != other(i, j))
          return false;
      }
    }

    return true;
  }

  bool operator!=(const matrix &other) const { return !(*this == other); }

  float &operator()(size_t x, size_t y);
  float operator()(size_t x, size_t y) const;

  matrix &operator*=(const matrix &other);
  matrix operator*(const matrix &other) const;
};
} // namespace math