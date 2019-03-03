#pragma once

#include "Mesh.h"
#include <algorithm>
class Cube : public Mesh {
  std::vector<Quad> quads;
  mutable std::vector<Quad> cached;

public:
  Cube() {
    // clang-format off
    quads = {
        //2 sides are redundant, can be removed?
        // SOUTH
        {
          math::vector(0.0f, 0.0f, 0.0f),
          math::vector(0.0f, 1.0f, 0.0f),
          math::vector(1.0f, 1.0f, 0.0f),
          math::vector(1.0f, 0.0f, 0.0f),
        },
        // NORTH
        {
          math::vector(1.0f, 0.0f, 1.0f),
          math::vector(1.0f, 1.0f, 1.0f),
          math::vector(0.0f, 1.0f, 1.0f),
          math::vector(0.0f, 0.0f, 1.0f),
        },
        // EAST
        {
          math::vector(1.0f, 0.0f, 0.0f),
          math::vector(1.0f, 1.0f, 0.0f),
          math::vector(1.0f, 1.0f, 1.0f),
          math::vector(1.0f, 0.0f, 1.0f),
        },

        // WEST
        {
          math::vector(0.0f, 0.0f, 1.0f),
          math::vector(0.0f, 1.0f, 1.0f),
          math::vector(0.0f, 1.0f, 0.0f),
          math::vector(0.0f, 0.0f, 0.0f),
        },
        // TOP
        {
          math::vector(0.0f, 1.0f, 0.0f),
          math::vector(0.0f, 1.0f, 1.0f),
          math::vector(1.0f, 1.0f, 1.0f),
          math::vector(1.0f, 1.0f, 0.0f),
        },
        // BOTTOM
        {
          math::vector(1.0f, 0.0f, 1.0f),
          math::vector(0.0f, 0.0f, 1.0f),
          math::vector(0.0f, 0.0f, 0.0f),
          math::vector(1.0f, 0.0f, 0.0f),
        }
    };
    // clang-format on
    cached = quads;
  }

  void draw(sf::RenderTarget &target, sf::RenderStates states) const override {
    Mesh::draw(cached, [&target](sf::Vertex *vertices, size_t num) {
      for (int i = 0; i < num; ++i) {
        vertices[i].color = sf::Color::White;
      }
      target.draw(vertices, 2, sf::Lines);
    });

    // reset transformations
    cached = quads;
  }

  void rotate(const Dimension rotation, float theta) override {
    switch (rotation) {
    case Dimension::X:
      rotate(math::make_rotation_x(theta));
      break;
    case Dimension::Y:
      rotate(math::make_rotation_z(theta));
      break;
    // TODO: Add Z rotation
    case Dimension::Z:
      rotate(math::make_rotation_z(theta));
      break;
    // TODO: Throw exception
    default:
      break;
    }
  }
  void rotate(const math::matrix &matrix) override {
    for (auto &polygon : cached) {
      for (auto &vertex : polygon.vertices) {
        vertex = math::multiply(vertex, matrix);
      }
    }
    // rotated_z.vertices[0] = math::multiply(tri.vertices[0], z_rotation);
  }
  void translate(const math::vector &vector) override {
    for (auto &polygon : cached) {
      for (auto &vertex : polygon.vertices) {
        vertex += vector;
      }
    }
  }

  void translate(const Dimension rotation, float delta) override {
    for (auto &polygon : cached) {
      for (auto &vertex : polygon.vertices) {
        switch (rotation) {
        case Dimension::X:
          vertex.x += delta;
          break;
        case Dimension::Y:
          vertex.y += delta;
          break;
        case Dimension::Z:
          vertex.z += delta;
          break;
        }
      }
    }
  }

  void scale(float scale) override {
    for (auto &polygon : cached) {
      for (auto &vertex : polygon.vertices) {
        vertex *= scale;
      }
    }
  }
  void scale(const Dimension rotation, float scale) override {
    for (auto &polygon : cached) {
      for (auto &vertex : polygon.vertices) {
        switch (rotation) {
        case Dimension::X:
          vertex.x *= scale;
          break;
        case Dimension::Y:
          vertex.y *= scale;
          break;
        case Dimension::Z:
          vertex.z *= scale;
          break;
        }
      }
    }
  }

  std::vector<Quad> vertices() const {
    return quads;
  }
};