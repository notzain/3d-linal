#pragma once

#include "Mesh.h"
#include "Cube.h"
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

  float yaw{};
  float pitch{};

  Camera(const CameraSettings &settings) : settings(settings) { reconfigure(); }

  void reconfigure() {
    projection = math::make_projection(settings.fov, settings.aspect_ratio,
                                       settings.near, settings.far);
  }

  void transform(Cube& mesh) {
	  const auto rotZ = math::make_rotation_z(pitch * .5f);
	  const auto rotX = math::make_rotation_x(pitch);

	  const auto translation = math::make_translation({ 0.f, 0.f, 5.f });

	  auto world = rotZ * rotX;
	  world *= translation;

	  math::vector up{ 0, 1, 0 };
	  math::vector target{ 0, 0, 1 };

	  const auto cameraRot = math::make_rotation_y(yaw);

	  auto look_dir = math::multiply(target, cameraRot);
	  target = camera_pos + look_dir;

	  const auto cameraMat = math::point_at(camera_pos, target, up);
	  const auto view = math::inverse(cameraMat);

	  for (auto& quad : mesh.cached) {
		  for (auto& vertex : quad.vertices) {
			  vertex = math::multiply(vertex, world);
			  vertex = math::multiply(vertex, view);
			  vertex = math::multiply(vertex, projection);

			  vertex = vertex / vertex.w;

			  vertex = vertex + math::vector{ 1,1,0 };
			  vertex.x *= (.5 * settings.screen_width);
			  vertex.y *= (.5 * settings.screen_height);
		  }
	  }
  }
};