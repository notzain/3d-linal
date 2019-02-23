#pragma once

#include "Mesh.h"
#include <sstream>
#include <fstream>
#include <string>

class Object : public Mesh {
  mutable std::vector<Triangle> cached;

public:
  Object(const std::string &filename) {
    load_from_file(filename);
    cached = triangles;
  }

  void draw(sf::RenderTarget &target, sf::RenderStates states) const override {
        Mesh::draw(cached, [&target](sf::Vertex *vertices, size_t num) {
      for (int i = 0; i < num; ++i) {
        vertices[i].color = sf::Color::Green;
      }
      target.draw(vertices, 2, sf::Lines);
    });
    // reset transformations
    cached = triangles;
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
        int f[3];
        s >> junk >> f[0] >> f[1] >> f[2];
        triangles.push_back(
            {verts[f[0] - 1], verts[f[1] - 1], verts[f[2] - 1]});
      }
    }
  }
};