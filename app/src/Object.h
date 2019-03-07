#pragma once

#include "math/math.hpp"
#include "Mesh.h"
#include <fstream>
#include <sstream>
#include <string>

class Object : public Mesh {
  math::vector origin_;
  math::vector rotation_;

  std::vector<Triangle> triangles;
  mutable std::vector<Triangle> cached_triangles;

  std::vector<Quad> quads;
  mutable std::vector<Quad> cached_quads;

public:
  Object(const std::string &filename) {
    load_from_file(filename);
    cached_triangles = triangles;
    cached_quads = quads;
  }

  void draw(sf::RenderTarget &target, sf::RenderStates states) const override {
    const auto draw_cb =
        [&target,
         color = sf::Color(color[0] * 255, color[1] * 255, color[2] * 255)](
            sf::Vertex *vertices, size_t num) {
          for (int i = 0; i < num; ++i) {
            vertices[i].color = color;
          }
          target.draw(vertices, num, sf::Lines);
        };

    Mesh::draw(cached_triangles, draw_cb);
    Mesh::draw(cached_quads, draw_cb);

    // reset transformations
    cached_triangles = triangles;
    cached_quads = quads;
  }

  math::vector &origin() override { return origin_; }
  math::vector origin() const override { return origin_; }

  math::vector &rotation() override { return rotation_; }
  math::vector rotation() const override { return rotation_; }

  void rotate(const math::matrix &matrix) override {
    for (auto &polygon : cached_quads) {
      for (auto &vertex : polygon.vertices) {
        vertex = math::multiply(vertex, matrix);
      }
    }
    for (auto &polygon : cached_triangles) {
      for (auto &vertex : polygon.vertices) {
        vertex = math::multiply(vertex, matrix);
      }
    }
  }
  void scale(const math::matrix &matrix) override {
    for (auto &polygon : cached_quads) {
      for (auto &vertex : polygon.vertices) {
        vertex = math::multiply(vertex, matrix);
      }
    }
    for (auto &polygon : cached_triangles) {
      for (auto &vertex : polygon.vertices) {
        vertex = math::multiply(vertex, matrix);
      }
    }
  }
  void translate(const math::matrix &matrix) override {
    for (auto &polygon : cached_quads) {
      for (auto &vertex : polygon.vertices) {
        vertex += math::vector(matrix(3, 0), matrix(3, 1), matrix(3, 2));
      }
    }
    for (auto &polygon : cached_triangles) {
      for (auto &vertex : polygon.vertices) {
        vertex += math::vector(matrix(3, 0), matrix(3, 1), matrix(3, 2));
      }
    }
  }
  void project(const math::matrix &matrix) override {
    for (auto &polygon : cached_quads) {
      for (auto &vertex : polygon.vertices) {
        vertex = math::multiply(vertex, matrix);
        vertex = vertex / vertex.w;
      }
    }
    for (auto &polygon : cached_triangles) {
      for (auto &vertex : polygon.vertices) {
        vertex = math::multiply(vertex, matrix);
        vertex = vertex / vertex.w;
      }
    }
  }

private:
  void load_from_file(const std::string &filename) {
    std::ifstream f(filename);
    if (!f.is_open())
      throw "Couldn't open file.";
    // Local cache of verts
    std::vector<math::vector> verts;

    while (!f.eof()) {
      char line[128];
      f.getline(line, 128);

      std::stringstream s;
      s << line;

      char junk;

      if (line[0] == 'v') {
        math::vector v;
        s >> junk >> v.x >> v.y >> v.z;
        verts.push_back(v);
      }

      if (line[0] == 'f') {
        const auto str = s.str();
        auto count = std::count(str.begin(), str.end(), ' ');
        if (count == 3) {
          int f[3];
          s >> junk >> f[0] >> f[1] >> f[2];
          triangles.push_back(
              {verts[f[0] - 1], verts[f[1] - 1], verts[f[2] - 1]});
        } else if (count == 4) {
          int f[4];
          s >> junk >> f[0] >> f[1] >> f[2] >> f[3];
          quads.push_back({verts[f[0] - 1], verts[f[1] - 1], verts[f[2] - 1],
                           verts[f[3] - 1]});
        }
      }
    }
  }
};