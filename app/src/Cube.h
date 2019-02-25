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
          0.0f, 0.0f, 0.0f,
          0.0f, 1.0f, 0.0f,
          1.0f, 1.0f, 0.0f,
          1.0f, 0.0f, 0.0f,
        },
        // NORTH
        {
          1.0f, 0.0f, 1.0f,
          1.0f, 1.0f, 1.0f,
          0.0f, 1.0f, 1.0f,
          0.0f, 0.0f, 1.0f,
        },
        // EAST
        {
          1.0f, 0.0f, 0.0f,
          1.0f, 1.0f, 0.0f,
          1.0f, 1.0f, 1.0f,
          1.0f, 0.0f, 1.0f,
        },

        // WEST
        {
          0.0f, 0.0f, 1.0f,
          0.0f, 1.0f, 1.0f,
          0.0f, 1.0f, 0.0f,
          0.0f, 0.0f, 0.0f,
        },
        // TOP
        {
          0.0f, 1.0f, 0.0f,
          0.0f, 1.0f, 1.0f,
          1.0f, 1.0f, 1.0f,
          1.0f, 1.0f, 0.0f,
        },
        // BOTTOM
        {
          1.0f, 0.0f, 1.0f,
          0.0f, 0.0f, 1.0f,
          0.0f, 0.0f, 0.0f,
          1.0f, 0.0f, 0.0f,
        }
    };
    // clang-format on
    cached = quads;
  }

  void draw(sf::RenderTarget &target, sf::RenderStates states) const override {

    /*
    std::vector<std::array<sf::Vector2f, 4>> planes;

    std::sort(cached.begin(), cached.end(), [](const Quad& a, const Quad& b){
      const auto z_avg = [](const Quad& quad) {
        float z = 0;
        for (const auto& vec : quad.vertices) {
          z += vec.z;
        }
        return z/4;
      };

      return z_avg(a) > z_avg(b);
    });
    */

    Mesh::draw(cached, [&target](sf::Vertex *vertices, size_t num) {
      /*
      static int i = 0;
      static std::array<sf::Vector2f, 4> plane;

      if (i == 0) {
        plane[0] = vertices[0].position;
        plane[1] = vertices[1].position;
        i++;
      } else if (i == 1) {
        plane[2] = vertices[0].position;
        plane[3] = vertices[1].position;

        i = 0;
        planes.push_back(plane);
      }
      */
      for (int i = 0; i < num; ++i) {
        vertices[i].color = sf::Color::White;
      }
      target.draw(vertices, 2, sf::Lines);
    });

    /*
        srand(time(NULL));
        for (const auto &plane : planes) {
          sf::ConvexShape convex;
          convex.setPointCount(4);
          convex.setFillColor(sf::Color(rand() % 255, rand() % 255, rand() %
       255));

          for (int i = 0; i < 4; ++i) {
            convex.setPoint(i, plane.at(i));
          }
          target.draw(convex);
        }
        */

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
};