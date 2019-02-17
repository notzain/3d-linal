#include "matrix.hpp"

namespace math {
float &matrix::operator()(std::size_t x, std::size_t y) { return matrix[x][y]; }
float matrix::operator()(std::size_t x, std::size_t y) const {
  return matrix[x][y];
}
} // namespace math