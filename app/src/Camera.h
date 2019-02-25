#pragma once

#include "Mesh.h"
#include "math/math.hpp"
#include "math/matrix.hpp"
#include "math/vector.hpp"

#include <cmath>
class Camera {
  math::matrix projection_matrix{};
  math::vector camera_pos{};
  math::vector look_dir{};

  float yaw{};
  float theta{};

public:
  Camera(float fov, float aspect_ratio, float near, float far) {
    const float fov_radians = 1.f / tanf(fov * 0.5 / 180.f * M_PI);

    projection_matrix(0, 0) = aspect_ratio * fov_radians;
    projection_matrix(1, 1) = fov_radians;
    projection_matrix(2, 2) = far / (far - near);
    projection_matrix(3, 2) = (-far * near) / (far - near);
    projection_matrix(2, 3) = 1.f;
    projection_matrix(3, 3) = 0.f;
  }

  void move(const math::vector &movement) { camera_pos += movement; }

  void strafe(float rotation) { yaw += rotation; }

  math::matrix look_at(const math::vector &target, const math::vector &up) {
    auto new_forward = target - camera_pos;
    new_forward.normalize();

    auto new_up = up - (new_forward * up.dot_product(new_forward));
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

    matrix(3, 0) = camera_pos.x;
    matrix(3, 1) = camera_pos.y;
    matrix(3, 2) = camera_pos.z;

    return matrix;
  }

  void transform(Mesh &mesh) {
    auto rotation_z = math::make_rotation_z(theta * 0.5f);
    auto rotation_x = math::make_rotation_x(theta);
    auto translation = math::make_translation({0.f, 0.f, 5.f});

    auto world_matrix = rotation_z * rotation_x;
    world_matrix *= translation;

    math::vector up{0, 1, 0};
    math::vector target{0, 0, 1};

    auto rotation_cam = math::make_rotation_y(yaw);
    
    look_dir = math::multiply(target, rotation_cam);
    target = camera_pos + look_dir;

    auto cam = look_at(target, up);
    auto view_matrix = math::inverse(cam);

    mesh.rotate(world_matrix);
    mesh.rotate(view_matrix);
    mesh.rotate(projection_matrix);

    mesh.translate({1, 1, 0});
    mesh.scale(Dimension::X, 0.5 * 800);
    mesh.scale(Dimension::Y, 0.5 * 600);
  }
};