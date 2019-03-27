#include "math.hpp"

// Need for MSVC
#define _USE_MATH_DEFINES
#include <cmath>

namespace math {

vector multiply(const vector &vec, const matrix &mat) {
  // clang-format off
  return {
    vec.x * mat(0, 0) + vec.y * mat(1, 0) + vec.z * mat(2, 0) + vec.w * mat(3, 0),
    vec.x * mat(0, 1) + vec.y * mat(1, 1) + vec.z * mat(2, 1) + vec.w * mat(3, 1),
    vec.x * mat(0, 2) + vec.y * mat(1, 2) + vec.z * mat(2, 2) + vec.w * mat(3, 2),
    vec.x * mat(0, 3) + vec.y * mat(1, 3) + vec.z * mat(2, 3) + vec.w * mat(3, 3)
  };
  // clang-format on
}

matrix make_identity() {
  matrix matrix;
  for (int i = 0; i < 4; ++i) {
    matrix(i, i) = 1.0f;
  }
  return matrix;
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
  temp(0, 0) = cosf(angle);
  temp(0, 2) = sinf(angle);
  temp(2, 0) = -sinf(angle);
  temp(1, 1) = 1.0f;
  temp(2, 2) = cosf(angle);
  temp(3, 3) = 1.0f;

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

matrix make_translation(const vector &translation) {
  matrix matrix;
  matrix(0, 0) = 1.0f;
  matrix(1, 1) = 1.0f;
  matrix(2, 2) = 1.0f;
  matrix(3, 3) = 1.0f;
  matrix(3, 0) = translation.x;
  matrix(3, 1) = translation.y;
  matrix(3, 2) = translation.z;
  return matrix;
}

matrix make_scaling(const math::vector &scale) {
  matrix matrix;
  matrix(0, 0) = scale.x;
  matrix(1, 1) = scale.y;
  matrix(2, 2) = scale.z;
  matrix(3, 3) = 1.f;
  return matrix;
}

matrix make_projection(float fov, float aspect_ratio, float near, float far) {
  const float fov_radians = to_radians(fov / 2);

  matrix matrix;
  matrix(0, 0) = aspect_ratio * fov_radians;
  matrix(1, 1) = fov_radians;
  matrix(2, 2) = far / (far - near);
  matrix(3, 2) = (-far * near) / (far - near);
  matrix(2, 3) = 1.f;
  matrix(3, 3) = 0.f;

  return matrix;
}
matrix inverse(const matrix &mat) {
  // https://www.mathsisfun.com/algebra/matrix-inverse-row-operations-gauss-jordan.html
  matrix temp;
  temp(0, 0) = mat(0, 0);
  temp(0, 1) = mat(1, 0);
  temp(0, 2) = mat(2, 0);
  temp(0, 3) = 0.0f;
  temp(1, 0) = mat(0, 1);
  temp(1, 1) = mat(1, 1);
  temp(1, 2) = mat(2, 1);
  temp(1, 3) = 0.0f;
  temp(2, 0) = mat(0, 2);
  temp(2, 1) = mat(1, 2);
  temp(2, 2) = mat(2, 2);
  temp(2, 3) = 0.0f;
  temp(3, 0) = -(mat(3, 0) * temp(0, 0) + mat(3, 1) * temp(1, 0) +
                 mat(3, 2) * temp(2, 0));
  temp(3, 1) = -(mat(3, 0) * temp(0, 1) + mat(3, 1) * temp(1, 1) +
                 mat(3, 2) * temp(2, 1));
  temp(3, 2) = -(mat(3, 0) * temp(0, 2) + mat(3, 1) * temp(1, 2) +
                 mat(3, 2) * temp(2, 2));
  temp(3, 3) = 1.0f;

  return temp;
}

matrix point_at(const math::vector &pos, const math::vector &target,
                const math::vector &up) {

  const auto forward = (target - pos).normalized();
  const auto new_up = (up - (forward * up.dot_product(forward))).normalized();
  const auto right = new_up.cross_product(forward);

  matrix matrix;
  matrix(0, 0) = right.x;
  matrix(0, 1) = right.y;
  matrix(0, 2) = right.z;
  matrix(0, 3) = 0.f;

  matrix(1, 0) = new_up.x;
  matrix(1, 1) = new_up.y;
  matrix(1, 2) = new_up.z;
  matrix(1, 3) = 0.f;

  matrix(2, 0) = forward.x;
  matrix(2, 1) = forward.y;
  matrix(2, 2) = forward.z;
  matrix(2, 3) = 0.f;

  matrix(3, 0) = pos.x;
  matrix(3, 1) = pos.y;
  matrix(3, 2) = pos.z;
  matrix(3, 3) = 1.f;

  return matrix;
}

float dot_product(const vector &a, const vector &b) { return a.dot_product(b); }

float to_radians(float degrees) { return (degrees * M_PI) / 180; }

} // namespace math