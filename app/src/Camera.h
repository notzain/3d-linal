#pragma once

#include "Cube.h"
#include "Mesh.h"
#include "math/math.hpp"
#include "math/matrix.hpp"
#include "math/vector.hpp"

#include <cmath>

struct CameraSettings {
  const float screen_width{};
  const float screen_height{};
  const float aspect_ratio{};

  float fov{};
  float near{};
  float far{};
};

// https://learnopengl.com/Getting-started/Camera
struct Camera {
  CameraSettings settings{};
  math::matrix projection{};
  math::vector position{};
  math::vector direction{0, 0, 1};
  const math::vector up{0, 1, 0};

  const Mesh *to_follow = nullptr;

  float yaw = 90.f;
  float pitch{};
  Camera(const CameraSettings &settings) : settings(settings) { reconfigure(); }

  void reconfigure() {
    projection = math::make_projection(settings.fov, settings.aspect_ratio,
                                       settings.near, settings.far);
  }

  void move_forward(float dt) { position += direction * dt; }
  void move_sideways(float dt) {
    position += (direction.cross_product(up) * dt);
  }
  void look_vertical(float dt) {
    pitch += dt;
    pitch = std::clamp(pitch, -89.f, 89.f);
  }

  void look_horizontal(float dt) { yaw += dt; }

  void follow(const Mesh *target) { to_follow = target; }

  void transform(Mesh &mesh) {
    const auto rotZ = math::make_rotation_z(0.f);
    const auto rotX = math::make_rotation_x(0.f);

    const auto translation = math::make_translation({0.f, 0.f, 5.f});

    auto world = rotZ * rotX;
    world *= translation;

    const auto front =
        math::vector{cos(math::to_radians(yaw)) * cos(math::to_radians(pitch)),
                     sin(math::to_radians(pitch)),
                     sin(math::to_radians(yaw)) * cos(math::to_radians(pitch))};

    direction = front.normalized();

    const auto look_at = position + direction;
    const auto view = math::inverse(math::point_at(
        position, to_follow ? to_follow->origin() : look_at, up));

    // in this case, also works without inverse.
    // results in flipped normals (might be ok for this program)
    // const auto view = math::inverse(cameraMat);
    // const auto view = cameraMat;

    // rotate mesh around origin first
    mesh.rotate(math::make_rotation_x(mesh.rotation().x));
    mesh.rotate(math::make_rotation_y(mesh.rotation().y));
    mesh.rotate(math::make_rotation_z(mesh.rotation().z));

    // Every mesh is positioned on (0,0,0).
    // Move the mesh to its desired position by translating it by its origin.
    mesh.translate(math::make_translation(mesh.origin()));

    mesh.rotate(world);

    mesh.calc_normal();

    mesh.rotate(view);
    mesh.project(projection);

    mesh.translate(math::make_translation({1, 1, 0}));
    mesh.scale(math::make_scaling(
        {.5f * settings.screen_width, .5f * settings.screen_height, 0}));
  }
};