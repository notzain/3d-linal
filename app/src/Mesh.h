#pragma once

#include <SFML/Graphics.hpp>
#include <algorithm>
#include <array>
#include <vector>

#include "Polygon.h"
#include "math/matrix.hpp"

enum class Dimension { X, Y, Z };

class Mesh : public sf::Drawable {
public:
  virtual void draw(sf::RenderTarget &target,
                    sf::RenderStates states) const = 0;

  virtual void rotate(const math::matrix& matrix) = 0;
  virtual void scale(const math::matrix& matrix) = 0;
  virtual void translate(const math::matrix& matrix) = 0;
  virtual void project(const math::matrix& matrix) = 0;



  //virtual const math::vector& origin() const = 0;

  /**
   * @brief Helper function to convert our Primitives to SFML lines.
   *
   * @tparam T
   * @param mesh
   * @return std::vector<std::array<sf::Vertex, 2>>
   */
  template <typename T>
  static std::vector<std::array<sf::Vertex, 2>>
  make_lines(const std::vector<T> &mesh) {
    std::vector<std::array<sf::Vertex, 2>> lines;
    const auto size = T::size;

    for (const auto &polygon : mesh) {
      for (int i = 0; i < size; ++i) {
        const auto &current_vertex = polygon.vertices[i];
        const auto &next_vertex =
            i == size - 1 ? polygon.vertices[0] : polygon.vertices[i + 1];

		if (current_vertex.w < 0 || next_vertex.w < 0) {
			continue;
		}

        lines.push_back({sf::Vector2f{current_vertex.x, current_vertex.y},
                         sf::Vector2f{next_vertex.x, next_vertex.y}});
      }
    }

    return lines;
  }

protected:
  /**
   * @brief Convert a vector of Triangles or Polygons to SFML drawable lines.
           User provides a callback that defines how a line should be drawn.
   *
   * @tparam T Polygon or Triangle
   * @tparam DrawFunc void (Vertex* vertex as array, int size of array)
   * @param mesh Vector of T
   * @param f callback
   */
  template <typename T, typename DrawFunc>
  static void draw(const std::vector<T> &mesh, DrawFunc &&f) {
    auto lines = make_lines(mesh);

    // erase duplicate lines
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