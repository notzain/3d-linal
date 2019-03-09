#pragma once

#include "Polygon.h"

#include <SFML/Graphics.hpp>

struct RenderType {
  static const uint8_t WIREFRAME = 1 << 0;
  static const uint8_t SOLID = 1 << 1;
};

class MeshRenderer {
protected:
  sf::RenderWindow *window;

public:
  MeshRenderer(sf::RenderWindow *window) : window(window) {}

  virtual void draw(const std::vector<Polygon> &mesh, sf::Color color) = 0;

protected:
  math::vector normal_of(const Polygon &polygon) {
    math::vector normal;

    return normal;
  }
};

class WireframeRenderer : public MeshRenderer {
  bool see_through = false;

public:
  WireframeRenderer(sf::RenderWindow *window, bool see_through)
      : MeshRenderer(window), see_through(see_through) {}

  void draw(const std::vector<Polygon> &mesh, sf::Color color) override {
    for (const auto &polygon : mesh) {
      auto normal = polygon.normal.normalized();

      if (normal.z < 0 || see_through) {
        for (int i = 0; i < polygon.vertices.size(); ++i) {
          const auto &current_vertex = polygon.vertices[i];
          const auto &next_vertex = i == polygon.vertices.size() - 1
                                        ? polygon.vertices[0]
                                        : polygon.vertices[i + 1];

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
};

class SolidRenderer : public MeshRenderer {
public:
  SolidRenderer(sf::RenderWindow *window) : MeshRenderer(window) {}

  void draw(const std::vector<Polygon> &mesh, sf::Color color) override {
    auto sorted_polygons = mesh;
    std::sort(sorted_polygons.begin(), sorted_polygons.end(),
              [](const Polygon &a, const Polygon &b) {
                float z1 = 0;
                float z2 = 0;
                for (const auto &vertex : a.vertices) {
                  z1 += vertex.z;
                }
                z1 /= a.vertices.size();
                for (const auto &vertex : b.vertices) {
                  z2 += vertex.z;
                }
                z2 /= b.vertices.size();
                return z1 > z2;
              });

    for (const auto &polygon : sorted_polygons) {
      auto normal = polygon.normal.normalized();

      if (normal.z < 0) {
        bool valid_shape = true;
        sf::ConvexShape shape(polygon.vertices.size());

        for (int i = 0; i < polygon.vertices.size(); ++i) {
          auto current_vertex = polygon.vertices[i];
          auto next_vertex = i == polygon.vertices.size() - 1
                                 ? polygon.vertices[0]
                                 : polygon.vertices[i + 1];

          if (current_vertex.w < 0 || next_vertex.w < 0) {
            valid_shape = false;
            break;
          }
          shape.setPoint(i, {current_vertex.x, current_vertex.y});
        }

        if (valid_shape) {
          math::vector light_dir{0.f, 0.f, -5.f};
          light_dir.normalize();

          auto brightness =
              std::clamp(light_dir.dot_product(normal), 0.3f, 1.f);

          shape.setFillColor(sf::Color(color.r * brightness,
                                       color.g * brightness,
                                       color.b * brightness));

          window->draw(shape);
        }
      }
    }
  }
};

class WireframeAndSolidRenderer : public MeshRenderer {
  WireframeRenderer wireframe;
  SolidRenderer solid;

public:
  WireframeAndSolidRenderer(sf::RenderWindow *window, bool see_through)
      : MeshRenderer(window), wireframe(window, see_through), solid(window) {}
  void draw(const std::vector<Polygon> &mesh, sf::Color color) override {
    solid.draw(mesh, color);
    wireframe.draw(mesh, sf::Color::Black);
  }
};
