#pragma once

#include <SFML/Graphics.hpp>
#include <algorithm>
#include <array>
#include <vector>

#include "Triangle.h"
#include "math/matrix.hpp"

enum class Dimension { X, Y, Z };

class Mesh : public sf::Drawable {
protected:
  std::vector<Triangle> triangles;

public:
  virtual void draw(sf::RenderTarget &target,
                    sf::RenderStates states) const = 0;
  virtual void rotate(const Dimension rotation, float theta) = 0;
  virtual void rotate(const math::matrix &matrix) = 0;
  virtual void translate(const math::vector &vector) = 0;
  virtual void translate(const Dimension rotation, float delta) = 0;
  virtual void scale(float scale) = 0;
  virtual void scale(const Dimension rotation, float scale) = 0;

protected:
  /**
   * @brief This function will convert a <Mesh> into <sf::Vertex>'s that can be
   * drawn by SFML.
   *
   * @tparam DrawFunc void (Vertex* vertex as array, int size of array)
   * @param mesh Mesh to draw
   * @param f DrawFunc
   */
  template <typename DrawFunc>
  static void draw(const std::vector<Triangle> &mesh, DrawFunc &&f) {
    std::vector<std::array<sf::Vertex, 2>> lines;
    
    for (const auto &polygon : mesh) {
      for (int i = 0; i < 3; ++i) {
        const auto &current_vertex = polygon.vertices[i];
        const auto &next_vertex =
            i == 2 ? polygon.vertices[0] : polygon.vertices[i + 1];

        lines.push_back({sf::Vector2f{current_vertex.x, current_vertex.y},
                         sf::Vector2f{next_vertex.x, next_vertex.y}});
      }
    }

    lines.erase(std::unique(lines.begin(), lines.end(),
                            [](const auto &a, const auto &b) {
                              return (a[0].position == b[0].position &&
                                      a[1].position == b[1].position) ||
                                     (a[0].position == b[1].position &&
                                      a[1].position == b[0].position);
                            }),
                lines.end());

    for (auto &vertex : lines) {
      f(vertex.data(), 2);
    }
  }
};