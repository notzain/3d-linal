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

struct Camera {
  CameraSettings settings{};
  math::matrix projection{};
  math::vector position{};
  math::vector direction{0, 0, 1};
  math::vector up{0, 1, 0};
  float yaw = 90.f;
  float pitch{};

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
  const Mesh *target_;
  float distance_to_target = 2;
  FollowCamera(const CameraSettings &settings, const Mesh *target)
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

  void recalculate() {
    float hor_distance = distance_to_target * cosf(math::to_radians(pitch));
    float vert_distance = distance_to_target * sinf(math::to_radians(pitch));

    float angle = target_->rotation().y;
    float offset_x = hor_distance * sin(angle); // sin(math::to_radians(angle));
    float offset_z = hor_distance * cos(angle); // cos(math::to_radians(angle));

    position.x = target_->origin().x - offset_x;
    position.y = target_->origin().y + vert_distance;
    position.z = target_->origin().z - offset_z;

    yaw = 90 + math::to_degrees(target_->rotation().y);
  }

  void transform(Mesh &mesh) override {

    recalculate();
    const auto rotZ = math::make_rotation_z(0.f);
    const auto rotX = math::make_rotation_x(0.f);

    const auto translation =
        math::make_translation({0.f, 0.f, distance_to_target});

    auto world = rotZ * rotX;
    world *= translation;

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

    mesh.scale(math::make_scaling((mesh.scaling())));

    recalculate();

    const auto front = math::vector{
        cosf(math::to_radians(yaw)) * cosf(math::to_radians(pitch)),
        sinf(math::to_radians(pitch)),
        sinf(math::to_radians(yaw)) * cosf(math::to_radians(pitch))};

    direction = front.normalized();

    const auto look_at = direction + position;
    const auto view = math::inverse(math::point_at(position, look_at, up));

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