#pragma once

#include "Mesh.h"
#include "math/math.hpp"
#include <fstream>
#include <sstream>
#include <string>

class Object : public Mesh {
  math::vector origin_;
  math::vector rotation_;

  std::vector<Polygon> polygons;
  mutable std::vector<Polygon> cached;

public:
  Object(const std::string &filename) {
    load_from_file(filename);
    cached = polygons;
  }

  void draw(MeshRenderer &renderer) const override {
    renderer.draw(cached,
                  sf::Color(color[0] * 255, color[1] * 255, color[2] * 255));

    cached = polygons;
  }

  math::vector &origin() override { return origin_; }
  math::vector origin() const override { return origin_; }

  math::vector &rotation() override { return rotation_; }
  math::vector rotation() const override { return rotation_; }

  void rotate(const math::matrix &matrix) override {
    for (auto &polygon : cached) {
      for (auto &vertex : polygon.vertices) {
        vertex = math::multiply(vertex, matrix);
      }
    }
  }
  void scale(const math::matrix &matrix) override {
    for (auto &polygon : cached) {
      for (auto &vertex : polygon.vertices) {
        vertex = math::multiply(vertex, matrix);
      }
    }
  }
  void translate(const math::matrix &matrix) override {
    for (auto &polygon : cached) {
      for (auto &vertex : polygon.vertices) {
        vertex += math::vector(matrix(3, 0), matrix(3, 1), matrix(3, 2));
      }
    }
  }
  void project(const math::matrix &matrix) override {
    for (auto &polygon : cached) {
      for (auto &vertex : polygon.vertices) {
        vertex = math::multiply(vertex, matrix);
        vertex = vertex / vertex.w;
      }
    }
  }

  void calc_normal() override {
    for (auto &polygon : cached) {
      polygon.calculate_normal();
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

	  // vertex data
      if (line[0] == 'v') {
        math::vector v;
        s >> junk >> v.x >> v.y >> v.z;
        verts.push_back(v);
      }

	  // face/polygon data
      if (line[0] == 'f') {
        const auto str = s.str();

		// vertices in polygon
        auto count = std::count(str.begin(), str.end(), ' ');

		/*
		* face data: f # # # ...
		* # = vertex index
		*/
        if (count == 3) {
          int f[3];
          s >> junk >> f[0] >> f[1] >> f[2];
          polygons.emplace_back(
              Polygon{{verts[f[0] - 1], verts[f[1] - 1], verts[f[2] - 1]}});
        } else if (count == 4) {
          int f[4];
          s >> junk >> f[0] >> f[1] >> f[2] >> f[3];
          polygons.emplace_back(Polygon{{verts[f[0] - 1], verts[f[1] - 1],
                                         verts[f[2] - 1], verts[f[3] - 1]}});
        }
      }
    }
  }
};
