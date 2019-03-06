#pragma once

#include "Mesh.h"
#include <algorithm>
class Cube : public Mesh {
  std::vector<Quad> quads;

public:
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


  void rotate(const math::matrix& matrix) override {
	  for (auto& polygon : cached) {
		  for (auto& vertex : polygon.vertices) {
			  vertex = math::multiply(vertex, matrix);
		  }
	  }
  }
  void scale(const math::matrix& matrix) override {
	  for (auto& polygon : cached) {
		  for (auto& vertex : polygon.vertices) {
			  vertex = math::multiply(vertex, matrix);
		  }
	  }
  }
  void translate(const math::matrix& matrix) override {
	  for (auto& polygon : cached) {
		  for (auto& vertex : polygon.vertices) {
			  vertex += math::vector(matrix(3, 0), matrix(3, 1), matrix(3, 2));

		  }
	  }
  }
  void project(const math::matrix& matrix) override  {
	  for (auto& polygon : cached) {
		  for (auto& vertex : polygon.vertices) {
			  vertex = math::multiply(vertex, matrix);
			  vertex = vertex / vertex.w;
		  }
	  }
  }
};