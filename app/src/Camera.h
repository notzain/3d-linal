#pragma once

#include "math/matrix.hpp"
#include "math/vector.hpp"

#include <cmath>
class Camera {
  math::matrix matrix_projection{};
  math::vector vector_camera{};
  math::vector vector_eye{};
  math::vector vector_up{};

  float yaw{};
  float theta{};

public:
  Camera(float fov, float aspect_ratio, float near, float far) {
    const float fov_radians = 1.f / tanf(fov * 0.5 / 180.f * M_PI);

    matrix_projection(0, 0) = aspect_ratio * fov_radians;
    matrix_projection(1, 1) = fov_radians;
    matrix_projection(2, 2) = far / (far - near);
    matrix_projection(3, 2) = (-far * near) / (far - near);
    matrix_projection(2, 3) = 1.f;
    matrix_projection(3, 3) = 0.f;
  }

  math::matrix look_at(const math::vector &target) {
    auto new_forward = target - vector_camera;
    new_forward.normalize();

    auto new_up =
        vector_up - (new_forward * vector_up.dot_product(new_forward));
    new_up.normalize();

    auto new_right = new_up.cross_product(new_forward);

    math::matrix matrix;
    matrix(0, 0) = new_right.x;
    matrix(0, 1) = new_right.y;
    matrix(0, 2) = new_right.z;

    matrix(1, 0) = new_up.x;
    matrix(1, 1) = new_up.y;
    matrix(1, 2) = new_up.z;

    matrix(2, 0) = new_forward.x;
    matrix(2, 1) = new_forward.y;
    matrix(2, 2) = new_forward.z;

    matrix(3, 0) = vector_camera.x;
    matrix(3, 1) = vector_camera.y;
    matrix(3, 2) = vector_camera.z;

    return matrix;
  }
};