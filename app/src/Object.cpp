#include "Object.h"
#include <fstream>
#include <sstream>

void Object::load_from_file(const std::string &filename) {
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

    /* vertex data
     * v x y z
     * store vertices in an array,
     * which can later be retrieved by index (1 based)
     */
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

      /* face data: f # # # ...
       * # = vertex index
       */
      if (count == 2) {
        int f[2];
        s >> junk >> f[0] >> f[1];
        polygons.emplace_back(Polygon{{verts[f[0] - 1], verts[f[1] - 1]}});
      } else if (count == 3) {
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
