#pragma once

#include "Polygon.h"

#include <SFML/Graphics.hpp>
#include <numeric>

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
    std::vector<Polygon> polygons_to_draw;

    for (const auto &polygon : mesh) {
      const auto &normal = polygon.normal;
      if (normal.z < 0) {
        for (int i = 0; i < polygon.vertices.size(); ++i) {
          auto current_vertex = polygon.vertices[i];
          auto next_vertex = i == polygon.vertices.size() - 1
                                 ? polygon.vertices[0]
                                 : polygon.vertices[i + 1];

          if (current_vertex.w < 0 || next_vertex.w < 0) {
            break;
          }

          polygons_to_draw.push_back(polygon);
        }
      }
    }

    std::sort(polygons_to_draw.begin(), polygons_to_draw.end(),
              [](const Polygon &a, const Polygon &b) {
                const auto z = [](float prev, const math::vector &v) {
                  return prev + v.z;
                };

                float az = std::accumulate(a.vertices.begin(), a.vertices.end(),
                                           0.f, z);
                float bz = std::accumulate(b.vertices.begin(), b.vertices.end(),
                                           0.f, z);

                return az / a.vertices.size() > bz / b.vertices.size();
              });

    for (const auto &polygon : polygons_to_draw) {
      sf::ConvexShape shape(polygon.vertices.size());
      for (int i = 0; i < polygon.vertices.size(); ++i) {
        shape.setPoint(i, {polygon.vertices[i].x, polygon.vertices[i].y});
      }

      auto normal = polygon.normal;
      math::vector light_dir{0.f, 0.f, -5.f};
      light_dir.normalize();

      auto brightness = std::clamp(light_dir.dot_product(normal), 0.3f, 1.f);

      shape.setFillColor(sf::Color(color.r * brightness, color.g * brightness,
                                   color.b * brightness));

      window->draw(shape);
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
