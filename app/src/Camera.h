#pragma once

#include "Mesh.h"
#include "math/math.hpp"
#include "math/matrix.hpp"
#include "math/vector.hpp"

#include <cmath>

struct CameraSettings {
  float screen_width{};

  float screen_height{};
  float aspect_ratio{};
  float fov{};

  float near{};
  float far{};
};

struct Camera {
  CameraSettings settings{};
  math::matrix projection{};
  math::vector camera_pos{};
  math::vector look_dir{};

  float yaw{};
  float theta{};

  Camera(const CameraSettings &settings) : settings(settings) { reconfigure(); }

  void reconfigure() {
    projection = math::make_projection(settings.fov, settings.aspect_ratio,
                                       settings.near, settings.far);
  }

  void transform(Mesh &mesh) {
    const auto rotZ = math::make_rotation_z(theta * 0.5f);
    const auto rotX = math::make_rotation_x(theta);

    const auto translation = math::make_translation({0.f, 0.f, 5.f});

    auto world = rotZ * rotX;
    world *= translation;

    math::vector up{0, 1, 0};
    const auto cameraRot = math::make_rotation_y(yaw);

    math::vector target{0, 0, 1};

    look_dir = math::multiply(target, cameraRot);
    target = camera_pos + look_dir;

    const auto cameraMat = math::point_at(camera_pos, target, up);
    const auto view = math::inverse(cameraMat);

    mesh.rotate(world);
    mesh.rotate(view);
    mesh.rotate(projection);

    mesh.translate({1, 1, 0});
    mesh.scale(Dimension::X, settings.screen_width * 0.5f);
    mesh.scale(Dimension::Y, settings.screen_height * 0.5f);
  }
};