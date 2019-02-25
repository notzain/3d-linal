#include "matrix.hpp"
#include <string>

namespace math {
float &matrix::operator()(std::size_t x, std::size_t y) {
  if (x >= 4 || y >= 4) {
    throw "Index out of bound. (x, y) = (" + std::to_string(x) + ", " +
        std::to_string(y) + ")";
  }

  return values[x][y];
}
float matrix::operator()(std::size_t x, std::size_t y) const {
  if (x >= 4 || y >= 4) {
    throw "Index out of bound. (x, y) = (" + std::to_string(x) + ", " +
        std::to_string(y) + ")";
  }

  return values[x][y];
}

matrix &matrix::operator*=(const matrix &other) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      (*this)(j, i) = (*this)(j, 0) * other(0, i) +
                      (*this)(j, 1) * other(1, i) +
                      (*this)(j, 2) * other(2, i) + 
                      (*this)(j, 3) * other(3, i);
    }
  }

  return *this;
}
matrix matrix::operator*(const matrix &other) const {
  matrix temp = *this;
  temp *= other;
  return temp;
}
} // namespace math