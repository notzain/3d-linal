#include "Bullet.h"

#define INIT_VELOCITY 5

Bullet::Bullet(math::vector start, math::vector direction, float velocity)
    : velocity(velocity + INIT_VELOCITY) {
  origin_ = start;
  rotation_ = direction;

  color[0] = 1;
  color[1] = 0;
  color[2] = 0;

  polygons = {
      // NORTH
      Polygon{{math::vector(0.0f, 0.0f, 0.0f), math::vector(0.0f, 1.0f, 0.0f),
               math::vector(1.0f, 1.0f, 0.0f), math::vector(1.0f, 0.0f, 0.0f)}},
      // SOUTH
      Polygon{{math::vector(1.0f, 0.0f, 1.0f), math::vector(1.0f, 1.0f, 1.0f),
               math::vector(0.0f, 1.0f, 1.0f), math::vector(0.0f, 0.0f, 1.0f)}},
      // EAST
      Polygon{{math::vector(1.0f, 0.0f, 0.0f), math::vector(1.0f, 1.0f, 0.0f),
               math::vector(1.0f, 1.0f, 1.0f), math::vector(1.0f, 0.0f, 1.0f)}},
      // WEST
      Polygon{{math::vector(0.0f, 0.0f, 1.0f), math::vector(0.0f, 1.0f, 1.0f),
               math::vector(0.0f, 1.0f, 0.0f), math::vector(0.0f, 0.0f, 0.0f)}},
      // TOP
      Polygon{{math::vector(0.0f, 1.0f, 0.0f), math::vector(0.0f, 1.0f, 1.0f),
               math::vector(1.0f, 1.0f, 1.0f), math::vector(1.0f, 1.0f, 0.0f)}},
      // BOTTOM
      Polygon{{math::vector(1.0f, 0.0f, 1.0f), math::vector(0.0f, 0.0f, 1.0f),
               math::vector(0.0f, 0.0f, 0.0f),
               math::vector(1.0f, 0.0f, 0.0f)}}};

  for (auto &polygon : polygons) {
    for (auto &vertex : polygon.vertices) {
      vertex += math::vector(-.5, -.5, 0);
    }
  }
  cached = polygons;
}

void Bullet::update(float dt) {
  auto direction = math::rotation_to_direction(direction_, rotation_);
  direction.normalize();
  origin_ += direction * velocity * dt;
}