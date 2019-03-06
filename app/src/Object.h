#pragma once

#include "Mesh.h"
#include <fstream>
#include <sstream>
#include <string>

class Object : public Mesh {
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
    const auto draw_cb = [&target](sf::Vertex *vertices, size_t num) {
      for (int i = 0; i < num; ++i) {
        vertices[i].color = sf::Color::Green;
      }
      target.draw(vertices, num, sf::Lines);
    };

    Mesh::draw(cached_triangles, draw_cb);
    Mesh::draw(cached_quads, draw_cb);

    // reset transformations
    cached_triangles = triangles;
    cached_quads = quads;
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