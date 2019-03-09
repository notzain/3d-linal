#pragma once

#include "Polygon.h"

#include <SFML/Graphics.hpp>

enum class RenderType { WIREFRAME, SOLID };

class MeshRenderer {
protected:
  sf::RenderWindow *window;

public:
  MeshRenderer(sf::RenderWindow *window) : window(window) {}

  virtual void draw(const std::vector<Quad> &mesh, sf::Color color) = 0;
  virtual void draw(const std::vector<Triangle> &mesh, sf::Color color) = 0;

protected:
  static std::vector<std::array<sf::Vertex, 2>>
  make_wireframe(const std::vector<Quad> &mesh) {
    std::vector<std::array<sf::Vertex, 2>> wireframe;

    for (const auto &polygon : mesh) {
      // https://stackoverflow.com/questions/9806630/calculating-the-vertex-normals-of-a-quad
      math::vector line_a = polygon.vertices[1] - polygon.vertices[0];
      math::vector line_b = polygon.vertices[3] - polygon.vertices[0];

      auto normal = line_a.cross_product(line_b).normalized();

      if (normal.z < 0) {
        for (int i = 0; i < 4; ++i) {
          const auto &current_vertex = polygon.vertices[i];
          const auto &next_vertex =
              i == 4 - 1 ? polygon.vertices[0] : polygon.vertices[i + 1];

          if (current_vertex.w < 0 || next_vertex.w < 0) {
            continue;
          }

          wireframe.push_back({sf::Vector2f{current_vertex.x, current_vertex.y},
                               sf::Vector2f{next_vertex.x, next_vertex.y}});
        }
      }
    }

    return wireframe;
  }

  static std::vector<std::array<sf::Vertex, 2>>
  make_wireframe(const std::vector<Triangle> &mesh) {
    std::vector<std::array<sf::Vertex, 2>> wireframe;
    return wireframe;
  }

  static std::vector<std::array<sf::Vertex, 2>>
  make_solid(const std::vector<Quad> &mesh) {
    std::vector<std::array<sf::Vertex, 2>> wireframe;

    for (const auto &polygon : mesh) {
      // https://stackoverflow.com/questions/9806630/calculating-the-vertex-normals-of-a-quad
      math::vector line_a = polygon.vertices[1] - polygon.vertices[0];
      math::vector line_b = polygon.vertices[3] - polygon.vertices[0];

      auto normal = line_a.cross_product(line_b).normalized();

      if (normal.z < 0) {
        for (int i = 0; i < 4; ++i) {
          const auto &current_vertex = polygon.vertices[i];
          const auto &next_vertex =
              i == 4 - 1 ? polygon.vertices[0] : polygon.vertices[i + 1];

          if (current_vertex.w < 0 || next_vertex.w < 0) {
            continue;
          }

          wireframe.push_back({sf::Vector2f{current_vertex.x, current_vertex.y},
                               sf::Vector2f{next_vertex.x, next_vertex.y}});
        }
      }
    }

    return wireframe;
  }
};

class WireframeRenderer : public MeshRenderer {
  bool see_through = false;

public:
  WireframeRenderer(sf::RenderWindow *window, bool see_through)
      : MeshRenderer(window), see_through(see_through) {}

  void draw(const std::vector<Quad> &mesh, sf::Color color) override {
    for (const auto &polygon : mesh) {
      // https://stackoverflow.com/questions/9806630/calculating-the-vertex-normals-of-a-quad
      math::vector line_a = polygon.vertices[1] - polygon.vertices[0];
      math::vector line_b = polygon.vertices[3] - polygon.vertices[0];
      auto normal = line_a.cross_product(line_b).normalized();

      // auto normal = polygon.normal.normalized();

      if (normal.z < 0 || see_through) {
        for (int i = 0; i < 4; ++i) {
          const auto &current_vertex = polygon.vertices[i];
          const auto &next_vertex =
              i == 4 - 1 ? polygon.vertices[0] : polygon.vertices[i + 1];

          if (current_vertex.w < 0 || next_vertex.w < 0) {
            continue;
          }

          std::array line = {
              sf::Vertex({current_vertex.x, current_vertex.y}, color),
              sf::Vertex({next_vertex.x, next_vertex.y}, color)};

          window->draw(line.data(), 2, sf::Lines);
        }
      }
    }
  }
  void draw(const std::vector<Triangle> &mesh, sf::Color color) override {
    auto lines = make_wireframe(mesh);

    // erase duplicate lines
    lines.erase(std::unique(lines.begin(), lines.end(),
                            [](const auto &a, const auto &b) {
                              return (a[0].position == b[0].position &&
                                      a[1].position == b[1].position) ||
                                     (a[0].position == b[1].position &&
                                      a[1].position == b[0].position);
                            }),
                lines.end());

    for (auto &line : lines) {
      for (auto &vertex : line) {
        vertex.color = color;
      }

      window->draw(line.data(), 2, sf::Lines);
    }
  }

private:
};

class SolidRenderer : public MeshRenderer {
public:
  SolidRenderer(sf::RenderWindow *window) : MeshRenderer(window) {}

  void draw(const std::vector<Quad> &mesh, sf::Color color) override {
    for (const auto &polygon : mesh) {
      // https://stackoverflow.com/questions/9806630/calculating-the-vertex-normals-of-a-quad
      // math::vector line_a = polygon.vertices[2] - polygon.vertices[0];
      // math::vector line_b = polygon.vertices[3] - polygon.vertices[1];

      // auto normal = line_a.normalized().cross_product(line_b.normalized());
      // normal.normalize();

      auto normal = polygon.normal.normalized();
      normal *= -1;

      if (normal.z < 0 || true) {
        bool valid_shape = true;
        sf::ConvexShape shape(4);
        for (int i = 0; i < 4; ++i) {
          const auto &current_vertex = polygon.vertices[i];
          const auto &next_vertex =
              i == 4 - 1 ? polygon.vertices[0] : polygon.vertices[i + 1];

          if (current_vertex.w < 0 || next_vertex.w < 0) {
            valid_shape = false;
            break;
          }
          shape.setPoint(i, {current_vertex.x, current_vertex.y});
        }

        if (valid_shape) {
          math::vector light_dir{0.f, 0.f, -1.f};
          light_dir.normalize();

          auto brightness = std::max(0.1f, normal.dot_product(light_dir));

          shape.setFillColor(sf::Color(color.r * brightness,
                                       color.g * brightness,
                                       color.b * brightness));

          window->draw(shape);
        }
      }
    }
  }
  void draw(const std::vector<Triangle> &mesh, sf::Color color) override {}
};