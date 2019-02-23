#include "math.hpp"
#include "matrix.hpp"
#include "vector.hpp"

#include <cmath>

namespace math {
vector multiply(const vector &vec, const matrix &mat) {
  vector temp{0, 0, 0};
  temp.x =
      vec.x * mat(0, 0) + vec.y * mat(1, 0) + vec.z * mat(2, 0) + mat(3, 0);
  temp.y =
      vec.x * mat(0, 1) + vec.y * mat(1, 1) + vec.z * mat(2, 1) + mat(3, 1);
  temp.z =
      vec.x * mat(0, 2) + vec.y * mat(1, 2) + vec.z * mat(2, 2) + mat(3, 2);

  float w =
      vec.x * mat(0, 3) + vec.y * mat(1, 3) + vec.z * mat(2, 3) + mat(3, 3);
  if (w != 0.0f) {
    temp /= w;
  }

  return temp;
}
matrix make_rotation_x(float angle) {
  matrix temp;
  temp(0, 0) = 1;
  temp(1, 1) = cosf(angle);
  temp(1, 2) = sinf(angle);
  temp(2, 1) = -sinf(angle);
  temp(2, 2) = cosf(angle);
  temp(3, 3) = 1;

  return temp;
}

matrix make_rotation_y(float angle) {
  matrix temp;
  temp(0,0) = cosf(angle);
  temp(0,2) = sinf(angle);
  temp(2,0) = -sinf(angle);
  temp(1,1) = 1.0f;
  temp(2,2) = cosf(angle);
  temp(3,3) = 1.0f;

  return temp;
}
matrix make_rotation_z(float angle) {
  matrix temp;
  temp(0, 0) = cosf(angle);
  temp(0, 1) = sinf(angle);
  temp(1, 0) = -sinf(angle);
  temp(1, 1) = cosf(angle);
  temp(2, 2) = 1;
  temp(3, 3) = 1;

  return temp;
}

float dot_product(const vector &a, const vector &b) { return a.dot_product(b); }

} // namespace math