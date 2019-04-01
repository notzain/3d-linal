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
  math::vector position{};
  math::vector direction{0, 0, 1};
  math::vector up{0, 1, 0};
  float yaw = 90.f;
  float pitch{};
  float roll{};

  Camera(const CameraSettings &settings) : settings(settings) {}
  virtual void reconfigure() = 0;
  virtual void move_forward(float dt) = 0;
  virtual void move_sideways(float dt) = 0;
  virtual void look_vertical(float dt) = 0;
  virtual void look_horizontal(float dt) = 0;
  virtual void transform(Mesh &mesh) = 0;
};

// https://learnopengl.com/Getting-started/Camera
struct FreeCamera : public Camera {
  FreeCamera(const CameraSettings &settings) : Camera(settings) {
    reconfigure();
  }

  void reconfigure() override {
    projection = math::make_projection(settings.fov, settings.aspect_ratio,
                                       settings.near, settings.far);
  }
  void move_forward(float dt) override { position += direction * dt; }
  void move_sideways(float dt) override {
    position += (direction.cross_product(up) * dt);
  }
  void look_vertical(float dt) override {
    pitch += dt;
    pitch = std::clamp(pitch, -89.f, 89.f);
  }

  void look_horizontal(float dt) override { yaw += dt; }
  void transform(Mesh &mesh) override {
    const auto rotZ = math::make_rotation_z(0.f);
    const auto rotX = math::make_rotation_x(0.f);

    const auto translation = math::make_translation({0.f, 0.f, 5.f});

    auto world = rotZ * rotX;
    world *= translation;

    const auto front = math::vector{
        cosf(math::to_radians(yaw)) * cosf(math::to_radians(pitch)),
        sinf(math::to_radians(pitch)),
        sinf(math::to_radians(yaw)) * cosf(math::to_radians(pitch))};

    direction = front.normalized();

    const auto look_at = position + direction;
    const auto view = math::inverse(math::point_at(position, look_at, up));

    // in this case, also works without inverse.
    // results in flipped normals (might be ok for this program)
    // const auto view = math::inverse(cameraMat);
    // const auto view = cameraMat;

    mesh.scale(math::make_scaling(mesh.scaling()));

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

struct FollowCamera : public Camera {
  Mesh *target_;
  float distance_to_target = 1;
  FollowCamera(const CameraSettings &settings, Mesh *target)
      : Camera(settings), target_(target) {
    reconfigure();
  }

  void reconfigure() override {
    projection = math::make_projection(settings.fov, settings.aspect_ratio,
                                       settings.near, settings.far);
  }
  void move_forward(float dt) override { position += direction * dt; }
  void move_sideways(float dt) override {
    position += (direction.cross_product(up) * dt);
  }
  void look_vertical(float dt) override {
    pitch += dt;
    pitch = std::clamp(pitch, -89.f, 89.f);
  }

  void look_horizontal(float dt) override { yaw += dt; }

  void transform(Mesh &mesh) override {
    const auto rotZ = math::make_rotation_z(0.f);
    const auto rotX = math::make_rotation_x(0.f);

    const auto translation = math::make_translation({0.f, 0.f, 5.f});

    auto world = rotZ * rotX;
    world *= translation;

    position = target_->origin();
    position -= math::rotation_to_direction({0, 0, 1}, target_->rotation())
                    .normalized() *
                distance_to_target;
    yaw = 90 + math::to_degrees(target_->rotation().y);
    pitch = 0 - math::to_degrees(target_->rotation().x);

    const auto front = math::vector{
        cosf(math::to_radians(yaw)) * cosf(math::to_radians(pitch)),
        sinf(math::to_radians(pitch)),
        sinf(math::to_radians(yaw)) * cosf(math::to_radians(pitch))};

    direction = front.normalized();

    const auto look_at = position + direction;

    const auto view = math::inverse(math::point_at(position, look_at, up));

    // in this case, also works without inverse.
    // results in flipped normals (might be ok for this program)
    // const auto view = math::inverse(cameraMat);
    // const auto view = cameraMat;

    mesh.scale(math::make_scaling(mesh.scaling()));

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

struct BirdCamera : public Camera {
  BirdCamera(const CameraSettings &settings) : Camera(settings) {
    reconfigure();
    up = {0, 0, -1};
  }

  void reconfigure() override {
    projection = math::make_projection(settings.fov, settings.aspect_ratio,
                                       settings.near, settings.far);
  }
  void move_forward(float dt) override { position += direction * dt; }
  void move_sideways(float dt) override {
    position += (direction.cross_product(up) * dt);
  }
  void look_vertical(float dt) override {
    pitch += dt;
    pitch = std::clamp(pitch, -89.f, 89.f);
  }

  void look_horizontal(float dt) override { yaw += dt; }
  void transform(Mesh &mesh) override {
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
    const auto view = math::inverse(math::point_at(position, look_at, up));

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
    mesh.scale(math::make_scaling((mesh.scaling())));
  }
};