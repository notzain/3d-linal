#pragma once

#include "Cube.h"
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

// https://learnopengl.com/Getting-started/Camera
struct Camera {
  CameraSettings settings{};

  math::matrix projection{};
  math::vector camera_pos{};

  float yaw{};
  float pitch{};

  Camera(const CameraSettings &settings) : settings(settings) { reconfigure(); }

  void reconfigure() {
    projection = math::make_projection(settings.fov, settings.aspect_ratio,
                                       settings.near, settings.far);
  }

  void transform(Mesh &mesh) {
    const auto rotZ = math::make_rotation_z(0.f);
    const auto rotX = math::make_rotation_x(0.f);

    const auto translation = math::make_translation({0.f, 0.f, 5.f});

    auto world = rotZ * rotX;
    world *= translation;

    math::vector up{0, 1, 0};
    math::vector target{yaw, pitch, 1};

    const auto cameraRot = math::make_rotation_y(0.f);

    auto look_dir = math::multiply(target, cameraRot);
    auto look_at = camera_pos + look_dir;

    const auto cameraMat = math::point_at(camera_pos, look_at, up);

    // in this case, also works without inverse.
    // results in flipped normals (might be ok for this program)
    const auto view = math::inverse(cameraMat);

    // Every mesh is positioned on (0,0,0).
    // Move the mesh to its desired position by translating it by its origin.
    mesh.translate(math::make_translation(mesh.origin()));

    mesh.rotate(math::make_rotation_x(mesh.rotation().x));
    mesh.rotate(math::make_rotation_y(mesh.rotation().y));
    mesh.rotate(math::make_rotation_z(mesh.rotation().z));

    mesh.rotate(world);
    mesh.rotate(cameraMat);
    mesh.project(projection);

    mesh.translate(math::make_translation({1, 1, 0}));
    mesh.scale(math::make_scaling(
        {.5f * settings.screen_width, .5f * settings.screen_height, 0}));
  }
};